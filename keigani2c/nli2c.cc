#include <napi.h>
#include "nli2c.h"


Napi::Object ki2c::Init(Napi::Env env, Napi::Object exports){
  Napi::Function func=
    DefineClass(env,
		"ki2c",
			{ 
      InstanceMethod("enable", &ki2c::enable),
      InstanceMethod("disable", &ki2c::disable),
      InstanceMethod("stop", &ki2c::stop),
      InstanceMethod("moveTo", &ki2c::moveTo),
      InstanceMethod("moveBy", &ki2c::moveBy),
		});
  Napi::FunctionReference* constructor = new Napi::FunctionReference();

  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("ki2c",func); 
  return exports;
}

ki2c::ki2c(const Napi::CallbackInfo& info)
  : Napi::ObjectWrap<ki2c>(info){
    
  int length = info.Length();
  
  if (length<=0 || !info[0].IsString() || !info[1].IsNumber()){
    return;
  }
  //const char* device = info[0].ToString().Utf8Value().c_str(); 
  //int8_t addr = info[1].ToNumber().Uint32Value();
  
}



Napi::Value ki2c::enable(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  int length = info.Length();
  if (length<=0 || !info[0].IsFunction()) {
      return Napi::String::New(env,"-1");  
  }
  Napi::Function cb = info[0].As<Napi::Function>();


  cb.Call(env.Global(), {Napi::String::New(env,"")}); 

  return Napi::Number::New(env,0);
}

Napi::Value ki2c::disable(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  int length = info.Length();
  if (length<=0 || !info[0].IsFunction()) {
      return Napi::String::New(env,"-1");  
  }
  Napi::Function cb = info[0].As<Napi::Function>();
  cb.Call(env.Global(), {Napi::String::New(env,"")}); 

  return Napi::Number::New(env,0);
}

Napi::Value ki2c::stop(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  int length = info.Length();
  if (length<=0 || !info[0].IsFunction()) {
      return Napi::String::New(env,"-1");  
  }
  Napi::Function cb = info[0].As<Napi::Function>();
  cb.Call(env.Global(), {Napi::String::New(env,"")}); 

  return Napi::Number::New(env,0);
}

Napi::Value ki2c::moveTo(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  int length = info.Length();
  if (length<=0 || !info[2].IsFunction()) {
      return Napi::String::New(env,"-1");  
  }
  Napi::Function cb = info[2].As<Napi::Function>();
  cb.Call(env.Global(), {Napi::String::New(env,"")}); 

  return Napi::Number::New(env,0);
}


Napi::Value ki2c::moveBy(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  int length = info.Length();
  if (length<=0 || !info[2].IsFunction()) {
      return Napi::String::New(env,"-1");  
  }
  Napi::Function cb = info[2].As<Napi::Function>();
  cb.Call(env.Global(), {Napi::String::New(env,"")}); 

  return Napi::Number::New(env,0);
}

Napi::Value ki2c::writeBytes(const Napi::CallbackInfo& info){
 
  Napi::Env env = info.Env();
  int length = info.Length();

  if (!info[2].IsFunction()) {
    return Napi::String::New(env,"-1");  
  }

  Napi::Function cb = info[2].As<Napi::Function>();

  if(length<=0 || !info[0].IsNumber() || !info[1].IsTypedArray()){
    cb.Call(env.Global(), {Napi::String::New(env,"Incorrect arguments")});
    return Napi::String::New(env,"-1");
  }
   
  cb.Call(env.Global(), {Napi::String::New(env,"")}); 

  return Napi::Number::New(env,0);

}



Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return ki2c::Init(env, exports);
}

NODE_API_MODULE(ki2c, InitAll)
