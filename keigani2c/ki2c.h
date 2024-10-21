#ifndef MI2C
#define MI2C

#include <napi.h>
#include <string.h>
#include "KM1_I2C.h"


class ki2c : public Napi::ObjectWrap<ki2c>{
 public:
  static Napi::Object Init(Napi::Env,Napi::Object exports);
  ki2c(const Napi::CallbackInfo& info);
private:
  Napi::Value enable(const Napi::CallbackInfo& info);
  Napi::Value disable(const Napi::CallbackInfo& info);
  Napi::Value stop(const Napi::CallbackInfo& info);
  Napi::Value moveTo(const Napi::CallbackInfo& info);
  Napi::Value moveBy(const Napi::CallbackInfo& info);
  Napi::Value readStatus(const Napi::CallbackInfo& info);
  Napi::Value getAngle(const Napi::CallbackInfo& info);
  Napi::Value getRPM(const Napi::CallbackInfo& info);
  Napi::Value getTorque(const Napi::CallbackInfo& info);


  float _angle;
  float _rpm;
  float _torque;
  bool _enabled;
  float _speed;
  std::string name;
  int id;
  KeiganMotor *km;
};
#endif