//================================pin declaration ================================
const int 
tank_high_ind=5,
tank_low_ind=6,
sump_high_ind=7,
sump_low_ind=4,
manual_mode=8,
auto_mode=9,
indicator=10,
relay=12,
tank_low=A0,
tank_high=A1,
sump_low=A2,
sump_high=A3;

int manual_buttonstate,auto_buttonstate,flag=0,data;
 

void setup()
 {
  //Tank Inputs
  pinMode(tank_low,INPUT);
  pinMode(tank_high,INPUT);
  //Sump Inputs
  pinMode(sump_low,INPUT);
  pinMode(sump_high,INPUT); 
  //Manual / Auto mode
  pinMode(manual_mode,INPUT);
  pinMode(auto_mode,INPUT);
  //Tank level indicators
  pinMode(tank_high_ind,OUTPUT);
  pinMode(tank_low_ind,OUTPUT);
  //Sump Level indicators
  pinMode(sump_high_ind,OUTPUT);
  pinMode(sump_low_ind,OUTPUT);
  //Relay
  pinMode(relay,OUTPUT);
  //motor status  
  pinMode(indicator,OUTPUT);  
  Serial.begin(9600);

  

  }

void loop() 
 {
  
  manual_buttonstate=digitalRead(manual_mode);
  auto_buttonstate=digitalRead(auto_mode);

  //Manual Mode
  if(manual_buttonstate==HIGH )
  {
    Serial.println("MANUAL MODE"); 
    level();
    motor_on();  
  }
  //Automatic Mode
  if(auto_buttonstate==HIGH ) 
  {
      Serial.println("AUTO MODE");    
      data = level();  
      Serial.println("Condition data"); 
      Serial.println(data); 
      if(data ==1 || data == 11 || data == 101 || data ==111)
      {
        if(data == 111 && flag == 1)
        {
          motor_on();
          Serial.println("SPL CASE AUTOMATIC");                   
        }
        else
        {        
        if(data ==1 || data == 11 || data == 101 )           
          {
            Serial.println("Automatic Mode Normal case");
            motor_on();
            flag=1;
           }
          else
          {
            Serial.println("Automatic mode normal case stop");
            motor_off(1); 
          }
        }        
      }
  
      else
      {
        //Sensor failure 
      if  ( data ==10 || data == 1010|| data == 1000 || data ==110 || data ==1001 || data ==1011 || data ==1110 )
      {
        Serial.println("Sensor failure");
        motor_off(0);              
      }      
      //0 || 100 || 1100 || 1101 || 1111
        else
      {
        if(data== 1111)
        {
          Serial.println("motor off spl case");
          motor_off(1);
          flag=0;
        }
        else
        {
          motor_off(1);        
          Serial.println("motor off");

        }         
      }
     }
   }
        
  

  // Sleep
  if(auto_buttonstate==LOW && manual_buttonstate==LOW)
  {
    Serial.println("Sleep Mode");
    level();
    motor_off(1);    
  }


  delay(2000);

 

}



//To turn on motor
void motor_on()
{
    Serial.println("Motor on Function");
    digitalWrite(relay,HIGH);
    digitalWrite(indicator,LOW);
 
}

//To turn off motor if flag is 0 sensor error ,if flag is 1 default function

void motor_off(int flag)
{
    
   
    if(flag)
    {
    Serial.println("Motor off Function");
    digitalWrite(indicator,HIGH); 
    digitalWrite(relay,LOW);   
    }
    else
    {
      Serial.println("Motor off  due to sensor fault");
      digitalWrite(relay,LOW);
      bli();
    }
}

//routine to see water level
int level()
{
  Serial.println("Level");
  int condition[4];
  bool tank_low_dat=0,tank_high_dat=0,sump_low_dat=0,sump_high_dat=0;
  int x=10,data=0;


  tank_low_dat=digitalRead(tank_low);
  tank_high_dat=digitalRead(tank_high);
  sump_low_dat=digitalRead(sump_low);
  sump_high_dat=digitalRead(sump_high); 
  
  

  digitalWrite(tank_high_ind,!(tank_high_dat));
  digitalWrite(tank_low_ind,!(tank_low_dat));    
  digitalWrite(sump_high_ind,!(sump_high_dat));
  digitalWrite(sump_low_ind,!(sump_low_dat));  



  Serial.print(tank_high_dat);
  Serial.print(tank_low_dat);
  Serial.print(sump_high_dat);
  Serial.print(sump_low_dat);

  Serial.println("");


  condition[3]=tank_high_dat;
  condition[2]=tank_low_dat;
  condition[1]=sump_high_dat;
  condition[0]=sump_low_dat;

  for( int i=0;i<=3;i++)
  {
    Serial.print(condition[i]);
  }

  Serial.println("");

  
     
  //To convert array to a integer

    for(int j=0;j<=3;j++)
    {
        if(j==0){
            data=condition[j];
            
        }
        else{
          data=data+(x*condition[j]);
          x=x*10;
        }
        
    }

    
    return data;
 
}

//Blink program
void bli()
{ 
  Serial.println("Blink");   
  int i;                   
  for (i=0;i<=5;i++)
  {
  digitalWrite(indicator, LOW);   
  delay(500);
  digitalWrite(indicator, HIGH);    
  delay(500);
  }   
  
}
