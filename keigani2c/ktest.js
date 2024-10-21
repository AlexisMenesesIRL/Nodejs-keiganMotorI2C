var ki2c = require('bindings')('ki2c').ki2c;

let motor = new ki2c(0x20,"anyname",0);
console.log(motor.getAngle());

setTimeout(()=>motor.enable((err)=>{
        if(!err)
            setTimeout(()=>
                motor.moveTo(100,100,(err)=>{
                    if(!err){
                        setTimeout(()=>{
                            motor.readStatus(()=>{                                    
                                    console.log(motor.getAngle());
                                    console.log(motor.getRPM());
                                    console.log(motor.getTorque());
                                    console.log(err);
                                    if(!err)
                                        setTimeout(()=>{
                                                        motor.moveTo(100,0,(err)=>{
                                                        if(!err)
                                                            setTimeout(()=>{    
                                                                motor.readStatus(()=>{                                    
                                                                        console.log(motor.getAngle());
                                                                        console.log(motor.getRPM());
                                                                        console.log(motor.getTorque());
                                                                        console.log(err);
                                                                    
                                                                        if(!err)
                                                                            setTimeout(()=>{motor.disable(function(err){})},5000);
                                                                });  
                                                            },5000);
                                                        }   
                                        )},5000);
                                });
                        },5000);
                    }
            })
            ,1000);

}),1000);


