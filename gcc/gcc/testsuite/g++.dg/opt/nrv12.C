// { dg-do compile }
// { dg-options "-O2 -w" }

// The tree level NVR was messing around with addressable and not oring with the already set one.

void Assert(bool* ignore=0);
struct Vector
{ 
  void Set3(float x, float y, float z);
  __attribute__((vector_size(16))) float vecf4;
};
struct Vector3 : public Vector
{
  Vector3(float _x, float _y, float _z);
};
inline Vector3::Vector3(float _x, float _y, float _z )
{
  Set3(_x, _y, _z);
}
Vector3 GetVelocity(const int& state);
struct MultiSpline
{
  Vector3 GetVelocity() const;
  int* positional; 
  int curPositional;
};
inline Vector3 MultiSpline::GetVelocity() const
{
  static bool ignore=false;
  Assert(&ignore);
  if (positional)
    return ::GetVelocity(curPositional);
  return Vector3(0,0,0);
}
void RigidBodySplineJointCallback(void)
{
  MultiSpline* multi ;
  Vector3 v = multi->GetVelocity();
}
