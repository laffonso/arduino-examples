while(Uno.available() > 0){  //enquanto a porta serial estiver recebendo algo
    caractere = Uno.read();    //coloca o próximo caractere na variável caractere
    if(caractere != '\n'){     //Se a variável não for um final de linha,
      aux += caractere;    //Concatena o caractere na string url.
    }
    else                                //Caso o caractere for final de linha continuar a execução do programa para envio ao banco de dados 
    {
      
    }
