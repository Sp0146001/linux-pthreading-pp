#include <cmath>
#include <cstring>
#include <iostream>
#include <random>
#include <pthread.h>

bool isInside(const double x, const double y, const double r)
{
  return (x * x) + (y * y) <= (r * r);
}

size_t calc(const double r, const size_t tests, const size_t seed)
{
  std::mt19937 gen(seed);
  std::uniform_real_distribution< double > dist(-r, r);
  size_t hits = 0;

  for (size_t i = 0; i < tests; ++i)
  {
    if (isInside(dist(gen), dist(gen), r))
    {
      ++hits;
    }
  }

  return hits;
}

void* runSample(void* data)
{
  const double *args = static_cast< const double* >(data);
  const size_t hits = calc(args[0], static_cast< size_t >(args[1]), static_cast< size_t >(args[2]));

  return reinterpret_cast< void* >(hits);
}

double calculateArea(const double r, const size_t threads, const size_t tests)
{
  const size_t tests_per_thread = tests / threads;
  const size_t remainder = tests % threads;

  pthread_t* thread_handles = new pthread_t[threads];
  double* thread_args = new double[threads * 3];

  for (size_t i = 0; i < threads; ++i)
  {
    thread_args[i * 3 + 0] = r;
    thread_args[i * 3 + 1] = static_cast< double >(tests_per_thread + (i < remainder ? 1 : 0));
    thread_args[i * 3 + 2] = static_cast< double >(i + 1);

    const int err = pthread_create(&thread_handles[i], nullptr, runSample, &thread_args[i * 3]);
    if (err != 0)
    {
      std::cerr << std::strerror(err) << '\n';
    }
  }

  size_t total_hits = 0;
  for (size_t i = 0; i < threads; ++i)
  {
    size_t exit_code[1] = {};
    const int err = pthread_join(thread_handles[i], reinterpret_cast< void** >(&exit_code));
    if (err != 0)
    {
      std::cerr << std::strerror(err) << '\n';
    }
    total_hits += exit_code[0];
  }

  delete[] thread_handles;
  delete[] thread_args;

  const double square_area = 4.0 * r * r;
  return (static_cast< double >(total_hits) / static_cast< double >(tests)) * square_area;
}
