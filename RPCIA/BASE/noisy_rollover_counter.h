#if !defined(NOISY_ROLLOVER_COUNTER_H)
#define NOISY_ROLLOVER_COUNTER_H

class NoisyRolloverCounter {
private:
    int max;
    int last_value;
    int last_true;

public:
    // Initialize the counter with a maximum value
    NoisyRolloverCounter(int max);

    // Get the true unrolled-over value
    int get(int value);
};

#endif // NOISY_ROLLOVER_COUNTER_H