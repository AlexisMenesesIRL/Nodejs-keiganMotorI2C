/*********************************************
 * Author: Alexis Meneses
 * Date: 05/05/2021
 * Description: This library is for connecting
 *    from raspberry pi to keihan motor using
 *    i2c communication.
 * 
 * 
*********************************************/
#include <napi.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include <string.h>
#include <vector>
#include "KM1_I2C.h"
#include "ki2c.h"

#include <iostream>



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
      InstanceMethod("readStatus", &ki2c::readStatus),
      InstanceMethod("getAngle", &ki2c::getAngle),
      InstanceMethod("getRPM", &ki2c::getRPM),
      InstanceMethod("getTorque", &ki2c::getTorque),

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
  
  if (length<=0 || !info[0].IsNumber() || !info[1].IsString() || !info[2].IsNumber() ){
    return;
  }
  
  //const char* device = info[0].ToString().Utf8Value().c_str();

  int8_t addr = info[0].ToNumber().Uint32Value();

  name = info[1].ToString();
  id = info[2].ToNumber();
  
  _enabled = false;  // Motor status
  _speed = 0;      // Initial speed
  
  _angle = 0;
  _rpm = 0;
  _torque = 0;

  km = new KeiganMotor(addr); 
}

Napi::Value ki2c::enable(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  int length = info.Length();
  if (length<=0 || !info[0].IsFunction()) {
      return Napi::String::New(env,"-1");  
  }
  Napi::Function cb = info[0].As<Napi::Function>();

   if(!_enabled){
       if(!km->enable()){
         cb.Call(env.Global(), {Napi::String::New(env,"Error writing data")});
         return Napi::Number::New(env,-1);
      }
      else{
        _enabled= true;
      }
  }

  // 

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
   if(_enabled){
         if(!km->disable()){
           cb.Call(env.Global(), {Napi::String::New(env,"Error writing data")});
           return Napi::Number::New(env,-1);
         } 
        else{
          _enabled= false;
        }
   }
  
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
  if(_enabled){
          if(!km->stop()){
           cb.Call(env.Global(), {Napi::String::New(env,"Error writing data")});
           return Napi::Number::New(env,-1);
         } 

  }
  cb.Call(env.Global(), {Napi::String::New(env,"")}); 

  return Napi::Number::New(env,0);
}

/*
The move command has two values input:
speed: The speed is on rotation/minute (rpm).
angle: The angle is on degrees.
callback: This is a callback function.
*/

Napi::Value ki2c::moveTo(const Napi::CallbackInfo& info){
  Napi::Env env = info.Env();
  int length = info.Length();
  if (length<=0 || !info[2].IsFunction()) {
      return Napi::String::New(env,"-1");  
  }
  Napi::Function cb = info[2].As<Napi::Function>();
   if(_enabled){
      if(!info[0].IsNumber() || !info[1].IsNumber()){
         cb.Call(env.Global(), {Napi::String::New(env,"Incorrect arguments")});
         return Napi::String::New(env,"-1");
       }

     if(_speed != info[0].As<Napi::Number>().FloatValue()){
       if(!km->speedRpm(info[0].As<Napi::Number>().FloatValue())){
           cb.Call(env.Global(), {Napi::String::New(env,"Error writing data")});
           return Napi::Number::New(env,-1);
          
       }

      _speed = info[0].As<Napi::Number>().FloatValue();      
  
     }

    if(!km->moveToPositionDegree(info[1].As<Napi::Number>().FloatValue())){
        cb.Call(env.Global(), {Napi::String::New(env,"Error writing data")});
        return Napi::Number::New(env,-1);
    }
   }
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
  if(_enabled){
    if(!info[0].IsNumber() || !info[1].IsNumber()){
      cb.Call(env.Global(), {Napi::String::New(env,"Incorrect arguments")});
      return Napi::String::New(env,"-1");
    }
     if(_speed != info[0].As<Napi::Number>().FloatValue()){
       if(!km->speedRpm(info[0].As<Napi::Number>().FloatValue())){
           cb.Call(env.Global(), {Napi::String::New(env,"Error writing data")});
           return Napi::Number::New(env,-1);
       }
      _speed = info[0].As<Napi::Number>().FloatValue();      
     }
    if(!km->moveByDistanceDegree(info[1].As<Napi::Number>().FloatValue())){
        cb.Call(env.Global(), {Napi::String::New(env,"Error writing data")});
        return Napi::Number::New(env,-1);
    }
   
  }
  cb.Call(env.Global(), {Napi::String::New(env,"")}); 

  return Napi::Number::New(env,0);
}

Napi::Value ki2c::readStatus(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  int length = info.Length();

  if ( length<=0 || !info[0].IsFunction()) {
    return Napi::String::New(env,"-1");  
  }

  Napi::Function cb = info[0].As<Napi::Function>();  
  
  if(!km->readMotorMeasurement()){
      cb.Call(env.Global(), {Napi::String::New(env,"Error writing data")});
      return Napi::Number::New(env,-1);
  }

  _angle = km->degree;
  _rpm = km->rpm;
  _torque = km->torque;

  cb.Call(env.Global(), {Napi::String::New(env,"")}); 

  return Napi::Number::New(env,0); 

}

Napi::Value ki2c::getAngle(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Number::New(env,_angle); 
}

Napi::Value ki2c::getRPM(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Number::New(env,_rpm); 
}

Napi::Value ki2c::getTorque(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Number::New(env,_torque); 
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return ki2c::Init(env, exports);
}

NODE_API_MODULE(ki2c, InitAll)








