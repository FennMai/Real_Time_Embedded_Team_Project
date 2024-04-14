#include "CarControl.hpp"
#include <cmath>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <pigpio.h>

// Singleton instance initialization
CarControl* CarControl::instance = nullptr;

static constexpr float speed_deg_per_sec_turn = 45.0;

// Constructor is private to enforce singleton pattern
CarControl::CarControl() : _motorAPin1(17), _motorAPin2(22), _motorBPin1(10), _motorBPin2(11),
                           _encoderPinA(5), _encoderPinB(13), _ppr(360), _wheelCircumference(31.4),
                           _xPos(0.0), _yPos(0.0), _heading(0.0), _pulseCountA(0), _pulseCountB(0),
                           _forwardBackwardSpeed(60), _turnSpeed(40) {
    _MS = Emakefun_MotorShield(); // Ensure this is correctly constructed
    _servo = nullptr; // Initial null setup, should be configured in initialize()
}

// Destructor
CarControl::~CarControl() {
    cleanup();
}

// Singleton access method
CarControl* CarControl::getInstance() {
    static CarControl instance; // Static instance for singleton
    return &instance;
}

// Initialize all components
void CarControl::initialize() {
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize GPIO\n";
        throw std::runtime_error("GPIO initialization failed");
    }
    setupServo();
    setupDCMotors();
    setupEncoders();
}

// Setup servo motor
void CarControl::setupServo() {
    _MS.begin(50);
    _servo = _MS.getServo(1);
    _servo->writeServo(90); // Center servo position
}

// Setup DC motors
void CarControl::setupDCMotors() {
    gpioSetMode(_motorAPin1, PI_OUTPUT);
    gpioSetMode(_motorAPin2, PI_OUTPUT);
    gpioSetMode(_motorBPin1, PI_OUTPUT);
    gpioSetMode(_motorBPin2, PI_OUTPUT);
}

void CarControl::applyMotorSpeed() {
    // Map the _forwardBackwardSpeed (which could be a value from 0-100) to PWM range (typically 0-255)
    int pwmValue = map(_forwardBackwardSpeed, 0, 100, 0, 255); 
    gpioPWM(_motorAPin1, pwmValue);
    gpioPWM(_motorBPin1, pwmValue);
}

int CarControl::map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Setup encoders
void CarControl::setupEncoders() {
    gpioSetMode(_encoderPinA, PI_INPUT);
    gpioSetPullUpDown(_encoderPinA, PI_PUD_UP);
    gpioSetISRFunc(_encoderPinA, EITHER_EDGE, 0, encoderISR);
    gpioSetMode(_encoderPinB, PI_INPUT);
    gpioSetPullUpDown(_encoderPinB, PI_PUD_UP);
    gpioSetISRFunc(_encoderPinB, EITHER_EDGE, 0, encoderISR);
}

// Encoder interrupt service routine
void CarControl::encoderISR(int gpio, int level, uint32_t tick) {
    CarControl* instance = getInstance(); // Get instance correctly
    if (gpio == instance->_encoderPinA) {
        instance->_pulseCountA.fetch_add(1, std::memory_order_relaxed);
    } else if (gpio == instance->_encoderPinB) {
        instance->_pulseCountB.fetch_add(1, std::memory_order_relaxed);
    }
}

// Get the total distance traveled
float CarControl::getDistanceTraveled() const {
    int totalPulses = _pulseCountA.load(std::memory_order_relaxed) + _pulseCountB.load(std::memory_order_relaxed);
    return calculateDistance(totalPulses);
}

float CarControl::calculateDistance(int pulses) const {
    // implementation goes here
    return (pulses / static_cast<float>(_ppr)) * _wheelCircumference;
}

// Move the car forward
void CarControl::moveForward(float distance_cm, std::function<void()> callback) {
    stop();
    applyMotorSpeed();
    int pulsesNeeded = static_cast<int>((distance_cm / _wheelCircumference) * _ppr);
    _pulseCountA = 0;
    gpioWrite(_motorAPin1, PI_HIGH);
    gpioWrite(_motorAPin2, PI_LOW);
    gpioWrite(_motorBPin1, PI_HIGH);
    gpioWrite(_motorBPin2, PI_LOW);
    std::thread([=]() {
        while (_pulseCountA.load(std::memory_order_relaxed) < pulsesNeeded);
        stopDCMotors();
        if (callback) callback();
    }).detach();
}

// Turn the car right
void CarControl::turnRight(int degrees, std::function<void()> callback) {
    stop();
    _heading += degrees;
    _heading = fmod(_heading, 360); // Normalize the angle

    if (_servo) {
        _servo->writeServo(160); // Adjust for a right turn
    }
    applyMotorSpeed(); // Ensure motors are ready to resume after turn
    std::this_thread::sleep_for(std::chrono::milliseconds(int(degrees / speed_deg_per_sec_turn * 1000)));

    if (_servo) {
        _servo->writeServo(90); // Reset to center position after the turn
    }

    std::cout << "Heading after right turn: " << _heading << " degrees\n";
    if (callback) callback();
}

// Move the car backwards
void CarControl::moveBackward(float distance_cm, std::function<void()> callback) {
    stop();
    applyMotorSpeed();
    int pulsesNeeded = static_cast<int>((distance_cm / _wheelCircumference) * _ppr);
    _pulseCountB = 0; // Reset pulse count B for backward movement
    gpioWrite(_motorAPin1, PI_LOW);
    gpioWrite(_motorAPin2, PI_HIGH);
    gpioWrite(_motorBPin1, PI_LOW);
    gpioWrite(_motorBPin2, PI_HIGH);
    std::thread([=]() {
        while (_pulseCountB.load(std::memory_order_relaxed) < pulsesNeeded); // Correct variable used
        stopDCMotors();
        if (callback) callback();
    }).detach();
}

// Turn the car left
void CarControl::turnLeft(int degrees, std::function<void()> callback) {
    stop();
    _heading -= degrees;
    _heading = fmod(_heading, 360); // Normalize the angle

    if (_servo) {
        _servo->writeServo(20); // Adjust for a right turn
    }
    applyMotorSpeed(); // Ensure motors are ready to resume after turn
    std::this_thread::sleep_for(std::chrono::milliseconds(int(degrees / speed_deg_per_sec_turn * 1000)));
    
    if (_servo) {
        _servo->writeServo(90); // Reset to center position after the turn
    }

    std::cout << "Heading after right turn: " << _heading << " degrees\n";
    if (callback) callback();
}

void CarControl::stop() {
    stopDCMotors();
}

// Stop DC motors specifically
void CarControl::stopDCMotors() {
    gpioWrite(_motorAPin1, PI_LOW);
    gpioWrite(_motorAPin2, PI_LOW);
    gpioWrite(_motorBPin1, PI_LOW);
    gpioWrite(_motorBPin2, PI_LOW);
}

// Cleanup resources and GPIO on object destruction
void CarControl::cleanup() {
    stopDCMotors();
    gpioTerminate();
}

// Set the speed for the motors
void CarControl::setSpeed(int forwardBackwardSpeed, int turnSpeed) {
    _forwardBackwardSpeed = forwardBackwardSpeed;
    _turnSpeed = turnSpeed;
    // Adjust motor PWM or similar here based on hardware capabilities
}

// Get current X position
float CarControl::getXPosition() const {
    return _xPos;
}

float CarControl::getYPosition() const {
    return _yPos;
}

// Get current heading
float CarControl::getCurrentAngle() const {
    return _heading;
}