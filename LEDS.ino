void ledsInit(){
 pinMode(LED13,OUTPUT);
 digitalWrite(LED13,LOW); 
}

void ledStatus(byte stat){
 digitalWrite(LED13,stat);  
}
