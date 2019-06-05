String texto;
char caractere;

while(Serial.available() > 0){  //enquanto a porta serial estiver recebendo algo
    caractere = Serial.read();    //coloca o próximo caractere na variável caractere
    if(caractere != '\n'){     //Se a variável não for um final de linha,
      texto += caractere;    //Concatena o caractere na string url.
    }
    else                                //Caso o caractere for final de linha continuar a execução do programa para envio ao banco de dados 
    {
      Serial.print(texto)
    }
