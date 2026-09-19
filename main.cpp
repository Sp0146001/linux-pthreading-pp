#include <pthread.h>
#include <iostream>
#include <random>
#include <cmath>
#include <cstring>


bool isInside(double x, double y, double r)
{
  return x * x + y * y <= r * r;
}
