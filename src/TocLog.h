#pragma once
#include <Arduino.h>
#include <vector>

const size_t MAX_LOG_LINES = 50;
extern std::vector<String> webLogBuffer;

class TocLog : public Print {
private:
    String currentLine; // Holds partial prints until a newline (\n) occurs

public:
    // Core character capturing handle
    virtual size_t write(uint8_t c) override;
    
    // String block optimization buffer
    virtual size_t write(const uint8_t *buffer, size_t size) override;

    // C-style variable argument string formatting parser
    size_t printf(const char *format, ...) __attribute__((format(printf, 2, 3)));
};

// Expose the global object instance to your project
extern TocLog toclog;
