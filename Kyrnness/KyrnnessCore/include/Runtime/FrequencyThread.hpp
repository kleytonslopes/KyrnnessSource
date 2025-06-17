#pragma once
#ifndef KYRNESS_FREQUENCY_THREAD_HPP
#define KYRNESS_FREQUENCY_THREAD_HPP

#include <thread>
#include <atomic>
#include <functional>
#include <chrono>
#include <iostream>

class TFrequencyThread
{
public:
    TFrequencyThread(double hz, std::function<void()> callback)
        : m_Hz(hz), m_Callback(callback), m_Running(false)
    {
    }

    ~TFrequencyThread()
    {
        Stop();
    }

    void Start()
    {
        if (m_Running)
            return;

        m_Running = true;
        m_Thread = std::thread([this]()
            {
                using namespace std::chrono;

                const double intervalMs = 1000.0 / m_Hz;
                auto nextTick = high_resolution_clock::now();

                while (m_Running)
                {
                    auto startTime = high_resolution_clock::now();

                    // Executa o callback
                    m_Callback();

                    nextTick += milliseconds((int64_t)intervalMs);

                    auto now = high_resolution_clock::now();
                    auto sleepDuration = duration_cast<milliseconds>(nextTick - now);

                    if (sleepDuration.count() > 0)
                    {
                        std::this_thread::sleep_for(sleepDuration);
                    }
                    else
                    {
                        // Está atrasado, ajusta o próximo tick
                        nextTick = high_resolution_clock::now();
                    }
                }
            });
    }

    void Stop()
    {
        if (!m_Running)
            return;

        m_Running = false;
        if (m_Thread.joinable())
        {
            m_Thread.join();
        }
    }

private:
    double m_Hz;
    std::function<void()> m_Callback;
    std::atomic<bool> m_Running;
    std::thread m_Thread;
};

#endif // KYRNESS_FREQUENCY_THREAD_HPP

