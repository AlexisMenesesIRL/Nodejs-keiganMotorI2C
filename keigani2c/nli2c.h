
#ifndef NLI2C
#define NLI2C
#include <napi.h>

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
  int fd;
  bool enabled;
  float speed;
  std::string name;
  int id;


};
#endif