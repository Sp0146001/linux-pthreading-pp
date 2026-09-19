#include <pthread.h>
#include <iostream>
#include <random>
#include <cmath>
#include <cstring>

bool isInside(double x, double y, double r)
{
  return x * x + y * y <= r * r;
}

size_t calc(double r, size_t tests, size_t seed)
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

void* sample(void* data)
{
  auto p = static_cast< double* >(data);
  size_t hits = calc(p[0], static_cast<size_t>(p[1]), static_cast<size_t>(p[2]));
  return reinterpret_cast< void* >(hits);
}