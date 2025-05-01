
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

class Position{
public:

  Position(){}
  Position(int x,int y){
    this->x = x;
    this->y = y;
  }
  int x;
  int y;
};


//classe responsável pela realização de uma jogada

//o valor de a corresponde ao jogador que fará a jogada nas posições
//definidas
class Action{
public:

  Action(int x1, int y1, int x2, int y2, char a){

    Position position01(x1,y1);
    Position position02(x2,y2);

    this->position1 = position01;
    this->position2 = position02;

    this->jogador = a;

    if( x1 == x2 and y1 == y2){
      cout<<"JOGADA INVÁLIDA!!"<<endl;
    }
  }

  Position position1;
  Position position2;

  char jogador;

};


class Estado{
public:

  Estado(){

      for(int i=0;i<3;i++){
        vector<char> linha = {};
        for(int i2 = 0; i2<3; i2++){
          linha.push_back(' ');
        }
        this->estado_atual_tabuleiro1.push_back(linha);
      }

      for(int i=0;i<3;i++){
        vector<char> linha = {};
        for(int i2 = 0; i2<3; i2++){
          linha.push_back(' ');
        }
        this->estado_atual_tabuleiro2.push_back(linha);
      }
  }

  Estado(char a){

        for(int i=0;i<3;i++){
          vector<char> linha = {};
          for(int i2 = 0; i2<3; i2++){
            linha.push_back(' ');
          }
          this->estado_atual_tabuleiro1.push_back(linha);
        }

        for(int i=0;i<3;i++){
          vector<char> linha = {};
          for(int i2 = 0; i2<3; i2++){
            linha.push_back(' ');
          }
          this->estado_atual_tabuleiro2.push_back(linha);
        }
        this->jogador_atual = a;
  }

  vector<vector<char>> estado_atual_tabuleiro1;
  vector<vector<char>> estado_atual_tabuleiro2;

  char jogador_atual;

  //cada jogada de cada jogador é um turno. jogar nos dois tabuleiros conta como um turno
  //o primeiro turno é o turno 0, pois todas as posições estão preenchidas com ' '
  //isso significa que o jogador_atual no turno 0 é O

  //o jogador atual pode ser calculado a a partir do turno, pois o O sempre começa
  //se o turno%2 == 0 então a vez é do O. Ou seja, o último a jogar foi o X
  //ou a partida está no início
  int get_turno(){
    int resultado  = 0;
    for(int i = 0;i<3;i++){
      for(int i2 = 0;i2<3;i2++){
        if(estado_atual_tabuleiro1[i][i2] != ' '){
          resultado++;
        }
      }
    }
    return resultado;
  }
};

int expoente(int a, int expoente){
    int resultado = 1;

    for(int i = 0; i< expoente; i++){
        resultado=resultado * a;
    }
    return resultado;
}


class Memorizado{
public:

    Memorizado(){

        //for(int i =0;i <387420489; i++){
        //   this->custos.push_back(-2);
        //}
        this->custos = vector<int>(387420489,-2);
    }

    vector<int> custos;

    int soma = (387420489)*-2;

    int get_index(Estado estado){
        vector<int> valores(18,0);

        int contador = 0;

        for(int i = 0; i < 3; i++){
            for(int i2 = 0; i2< 3; i2++){

                if(estado.estado_atual_tabuleiro1[i][i2] == ' '){
                    valores[contador] = 0;
                }
                else if(estado.estado_atual_tabuleiro1[i][i2] == 'X'){
                    valores[contador] = 1;
                }
                else if(estado.estado_atual_tabuleiro1[i][i2] == 'O'){
                    valores[contador] = 2;
                }
                contador++;
            }
        }

        for(int i = 0; i < 3; i++){
            for(int i2 = 0; i2< 3; i2++){

                if(estado.estado_atual_tabuleiro2[i][i2] == ' '){
                    valores[contador] = 0;
                }
                else if(estado.estado_atual_tabuleiro2[i][i2] == 'X'){
                    valores[contador] = 1;
                }
                else if(estado.estado_atual_tabuleiro2[i][i2] == 'O'){
                    valores[contador] = 2;
                }
                contador++;
            }
        }

        int resultado =  0;
        for(int i = 0; i<18;i++){
            resultado += valores[i] * (expoente(3,i));
        }

        if(resultado>=custos.size() || resultado <0){
            cout << "Índice fora do vetor: " << resultado << endl;
        }
        return resultado;
    }


    bool ja_calculou(Estado estado){

        int index = get_index(estado);

        if(this->custos[index] == -2){
          //cout<<" não passou pela posição ainda" <<endl;
            return false;
        }
        else{
          //cout<<" já passou pela posição "<<endl;
            return true;
        }

    }

    int valor(Estado estado){
        int index = get_index(estado);

        if(this->custos[index] == -2){
            cout<<"ERRO!!! tentou buscar um valor que ainda não foi salvo. "<<endl<<endl;
        }

        return this->custos[index];
    }

    void salvar_valor(int number, Estado estado){
        int index = get_index(estado);

        this->soma +=2;

        if(this->soma >= 0){
          cout<<"teste todos os casos foram testados"<<endl;
        }

        this->custos[index] = number;

    }

};

Memorizado memory;

class Jogo_da_velha_modificado{
public:

    Jogo_da_velha_modificado(){

      char jogador_atual = 'O';

      //O COMPUTADOR COMEÇA JOGANDO DE CÍRCULO

      Estado estado(jogador_atual);

      this->estado_atual = estado;

    }


    Jogo_da_velha_modificado(Estado estado){

      this->estado_atual = estado;

    }



    //' ' igual a não preenchido. 1 = O; 2 = X
    Estado estado_atual;

    //O ou X
    char get_jogador_atual(Estado estado){
      return estado.jogador_atual;
    }

    //o turno é incrementado a cada jogada feita
    int get_turno(Estado estado){
      return estado.get_turno();
    }

    //posições possíveis de ser marcadas a partir do estado atual
    vector<Action> actions;



    //MÉTODOS DA CLASSE
    void print(){
        cout<<"____________________________________________"<<endl<<endl;
        for(int i = 0;i<3;i++){
          cout<<"[";
            for(int i2 = 0; i2<2; i2++){
                cout<< this->estado_atual.estado_atual_tabuleiro1[i][i2]<<"| ";
            }
            cout<< this->estado_atual.estado_atual_tabuleiro1[i][2];
          cout<<"]"<<endl;
        }
        cout<<endl<<endl;
        for(int i = 0;i<3;i++){
          cout<<"[";
            for(int i2 = 0; i2<2; i2++){
                cout<< this->estado_atual.estado_atual_tabuleiro2[i][i2]<<"| ";
            }
            cout<< this->estado_atual.estado_atual_tabuleiro2[i][2];
          cout<<"]"<<endl;
        }

        cout<<"____________________________________________"<<endl<<endl;

    }


    //preenche actions com todas as ações disponíveis
    vector<Action> generate_actions(Estado estado){
      char jogador;
      //no caso do primeiro ser O
      if(estado.get_turno()%2 == 0){
        jogador = 'O';
      }
      else{
        jogador = 'X';
      }

      vector<Action> actions = {};

      //posições que dá para marcar

      vector<vector<bool>> positions_tabuleiro1 = {
        {false,false,false},
        {false,false,false},
        {false,false,false}
      };
      vector<vector<bool>> positions_tabuleiro2 = {
        {false,false,false},
        {false,false,false},
        {false,false,false}
      };

      for( int i = 0; i < 3; i++ ){
        for(int i2 = 0; i2<3; i2++){
          if(estado.estado_atual_tabuleiro1[i][i2] == ' '){
            positions_tabuleiro1[i][i2] = true;
          }
        }
      }

      for( int i = 0; i < 3; i++ ){
        for(int i2 = 0; i2<3; i2++){
          if(estado.estado_atual_tabuleiro2[i][i2] == ' '){
            positions_tabuleiro2[i][i2] = true;
          }
        }
      }

      for( int i = 0; i < 3; i++ ){
        for(int i2 = 0; i2< 3; i2++){
          for( int i3 = 0; i3 < 3; i3++){
            for(int i4 = 0; i4 < 3; i4++){
              //não é permitido marcar na mesma posição nos dois tabuleiros
              if(i != i3 || i2 != i4){
                if(positions_tabuleiro1[i][i2]){
                  if(positions_tabuleiro2[i3][i4]){
                    actions.push_back(Action(i, i2, i3, i4, jogador));
                  }
                }
              }
            }
          }
        }
      }
      return actions;
    }



    void copiar_tabuleiro(vector<vector<char>>& tabuleiro_destino, vector<vector<char>> tabuleiro2){
      for(int i = 0;i<3;i++){
        for(int i2=0; i2<3; i2++){
          tabuleiro_destino[i][i2] = tabuleiro2[i][i2];
        }
      }
    }

    Estado copiar_estado(Estado estado1){

      Estado jogo2;

      copiar_tabuleiro(jogo2.estado_atual_tabuleiro1,estado1.estado_atual_tabuleiro1);
      copiar_tabuleiro(jogo2.estado_atual_tabuleiro2,estado1.estado_atual_tabuleiro2);

      return jogo2;

    }




    //retorna os tabuleiros após fazer a jogada descrita em action
    Estado action_result(Estado estado_inicial, Action action){

      Estado estado2;

      estado2 = copiar_estado(estado_inicial);

      estado2.estado_atual_tabuleiro1[action.position1.x][action.position1.y] = action.jogador;
      estado2.estado_atual_tabuleiro2[action.position2.x][action.position2.y] = action.jogador;

      estado2.jogador_atual = this->inverter_jogador(action.jogador);

      return estado2;

    }

    //Como a IA  joga de O, ela quer maximizar o custo

    //0 = empate ou partida não terminou   -1 = X   1 = O
    int custo_tabuleiro(vector<vector<char>> estado_atual_tabuleiro){


      bool coluna1 = (estado_atual_tabuleiro[0][0] == 'O' and estado_atual_tabuleiro[1][0] == 'O' and estado_atual_tabuleiro[2][0] == 'O');
      bool coluna2 = (estado_atual_tabuleiro[0][1] == 'O' and estado_atual_tabuleiro[1][1] == 'O' and estado_atual_tabuleiro[2][1] == 'O');
      bool coluna3 = (estado_atual_tabuleiro[0][2] == 'O' and estado_atual_tabuleiro[1][2] == 'O' and estado_atual_tabuleiro[2][2] == 'O');

      bool linha1 = (estado_atual_tabuleiro[0][0] == 'O' and estado_atual_tabuleiro[0][1] == 'O' and estado_atual_tabuleiro[0][2] == 'O');
      bool linha2 = (estado_atual_tabuleiro[1][0] == 'O' and estado_atual_tabuleiro[1][1] == 'O' and estado_atual_tabuleiro[1][2] == 'O');
      bool linha3 = (estado_atual_tabuleiro[2][0] == 'O' and estado_atual_tabuleiro[2][1] == 'O' and estado_atual_tabuleiro[2][2] == 'O');

      bool diagonal1 = (estado_atual_tabuleiro[0][0] == 'O' and estado_atual_tabuleiro[1][1] == 'O' and estado_atual_tabuleiro[2][2] == 'O');
      bool diagonal2 = (estado_atual_tabuleiro[0][2] == 'O' and estado_atual_tabuleiro[1][1] == 'O' and estado_atual_tabuleiro[2][0] == 'O');

      if(coluna1 || coluna2 || coluna3 || linha1 || linha2 || linha3 || diagonal1 || diagonal2){
        //jogador O ganhou
        return 1;
      }

      coluna1 = (estado_atual_tabuleiro[0][0] == 'X' and estado_atual_tabuleiro[1][0] == 'X' and estado_atual_tabuleiro[2][0] == 'X');
      coluna2 = (estado_atual_tabuleiro[0][1] == 'X' and estado_atual_tabuleiro[1][1] == 'X' and estado_atual_tabuleiro[2][1] == 'X');
      coluna3 = (estado_atual_tabuleiro[0][2] == 'X' and estado_atual_tabuleiro[1][2] == 'X' and estado_atual_tabuleiro[2][2] == 'X');

      linha1 = (estado_atual_tabuleiro[0][0] == 'X' and estado_atual_tabuleiro[0][1] == 'X' and estado_atual_tabuleiro[0][2] == 'X');
      linha2 = (estado_atual_tabuleiro[1][0] == 'X' and estado_atual_tabuleiro[1][1] == 'X' and estado_atual_tabuleiro[1][2] == 'X');
      linha3 = (estado_atual_tabuleiro[2][0] == 'X' and estado_atual_tabuleiro[2][1] == 'X' and estado_atual_tabuleiro[2][2] == 'X');

      diagonal1 = (estado_atual_tabuleiro[0][0] == 'X' and estado_atual_tabuleiro[1][1] == 'X' and estado_atual_tabuleiro[2][2] == 'X');
      diagonal2 = (estado_atual_tabuleiro[0][2] == 'X' and estado_atual_tabuleiro[1][1] == 'X' and estado_atual_tabuleiro[2][0] == 'X');

      if(coluna1 || coluna2 || coluna3 || linha1 || linha2 || linha3 || diagonal1 || diagonal2){
        //jogador X ganhou
        return -1;
      }

      //se não passou por nenhum dos dois if
      return 0;
    }

    //Retorna 1 se O ganhou, -1 se X ganhou, 0 caso contrário.
    int custo(const Estado& estado){
      if(custo_tabuleiro(estado.estado_atual_tabuleiro1) == 1 || custo_tabuleiro(estado.estado_atual_tabuleiro2) == 1){
        return 1;
      }
      if(custo_tabuleiro(estado.estado_atual_tabuleiro1) == -1 || custo_tabuleiro(estado.estado_atual_tabuleiro2) == -1){
        return -1;
      }
      else{
        return 0;
      }
    }



    //função não é utilizada
    char ganhador(const Estado& estado){
      if(custo(estado) == -1){
        return 'O';
      }
      else if(custo(estado) == 1){
        return 'X';
      }
    }



    //retorna verdadeiro se o jogo acabou
    bool final(Estado estado){
     /*  if(estado_atual[0][0] == ' ' || estado_atual[0][1] == ' ' || estado_atual[0][2] == ' ' ||
      estado_atual[1][0] == ' ' || estado_atual[1][1] == ' ' || estado_atual[1][2] == ' ' ||
      estado_atual[2][0] == ' ' || estado_atual[2][1] == ' ' || estado_atual[2][2] == ' ' ){
        return false;
      }
      else{
        return true;
      }*/
      if( estado.get_turno() == 9){
          return true;
      }
      else{
        if(custo(estado) != 0){
          return true;
        }
        else{
          vector<Action> actions = this->generate_actions(estado);
          if(actions.size() == 0){
            return true;
          }
          else{
            return false;
          }
        }
      }

    }

    void realizar_jogada(Action action, Estado& estado){
      estado.estado_atual_tabuleiro1[action.position1.x][action.position1.y] = action.jogador;
      estado.estado_atual_tabuleiro2[action.position2.x][action.position2.y] = action.jogador;
    }

    void jogada_computador_random(Estado& estado){

      srand(time(0));

      vector<Action> actions = this->generate_actions(estado);

      int sorteado = rand()%(actions.size());

      realizar_jogada(actions[sorteado], estado);
      /*
      estado.estado_atual_tabuleiro1[actions[sorteado].position1.x][actions[sorteado].position1.y] = 'O';
      estado.estado_atual_tabuleiro2[actions[sorteado].position2.x][actions[sorteado].position2.y] = 'O';
      */

    }

    bool jogada_permitida(const Estado& estado, const Action& action){

      bool resultado = false;

      if(action.position1.x != action.position2.x || action.position1.y != action.position2.y){
        if(action.position1.x>= 0 and action.position1.x <3 and
          action.position1.y >= 0 and action.position1.y <3 and
          action.position2.x >=0 and action.position2.x <3 and
          action.position2.y >= 0 and action.position2.y <3
        ){
          if(estado.estado_atual_tabuleiro1[action.position1.x][action.position1.y] == ' '
          and estado.estado_atual_tabuleiro2[action.position2.x][action.position2.y] == ' '
          ){
            resultado = true;
          }
        }
      }

      return resultado;

    }

    void jogada_jogador(Estado& estado){
      /*
      srand(time(0));

      vector<Action> actions = this->generate_actions(estado);

      int sorteado = rand()%(actions.size());

      estado.estado_atual_tabuleiro1[actions[sorteado].position1.x][actions[sorteado].position1.y] = 'X';
      estado.estado_atual_tabuleiro2[actions[sorteado].position2.x][actions[sorteado].position2.y] = 'X';
      */
      int jogada[4] = {0,0,0,0};
      cout<<"DIGITE SUA JOGADA NO SEGUINTE FORMATO: "<<endl;
      cout<<"(linha tabuleiro 1) (coluna tabuleiro 1) (linha tabuleiro 2) (coluna tabuleiro 2)"<<endl;

      bool jogada_feita = false;

      while(! jogada_feita){

        for(int i = 0;i<4;i++){
          cin>>jogada[i];
          jogada[i]--;
        }
        if(jogada_permitida(estado,Action(jogada[0],jogada[1],jogada[2],jogada[3],'X'))){
          jogada_feita = true;
          realizar_jogada(Action(jogada[0],jogada[1],jogada[2],jogada[3],'X'), estado);
        }
        else{
          cout<<"DIGITE UMA JOGADA VÁLIDA! "<<endl;
        }
      }
    }

    char inverter_jogador(char jogador){
      if(jogador == 'X'){
        return 'O';
      }
      else{
        return 'X';
      }
    }


    int maior(vector<int> numbers){
      int maior = -1;
      for(int i = 0;i <numbers.size(); i++){
        if( numbers[i] > maior){
          maior = numbers[i];
        }
      }
      return maior;
    }

    int menor(vector<int> numbers){
      int menor = 1;
      for(int i = 0;i <numbers.size(); i++){
        if( numbers[i] < menor){
          menor = numbers[i];
        }
      }
      return menor;
    }








    //o computador quer maximixar O e joga contra alguém que quer minimizar O

    //o jogador quer minimizar custo()
    //retorna 1 se jogador perde, 0 se ele empata e -1 se consegue ganhar
    int min(Estado estado, char jogador){

      //antes de fazer as operações eu passo pela lista memorizada
      if(memory.ja_calculou(estado)){
        return memory.valor(estado);
      }

      if( this->final(estado) ){
      //se estamos no final do jogo sabemos se a jogada que levou a esse estado é boa ou não
      //se X ganhou o custo é -1, se foi empate o custo é 0 e se X perdeu o custo é 1
          memory.salvar_valor(custo(estado),estado);
          return custo(estado);
      }
      else{
        //nesse caso geramos as jogadas possíveis e testamos cada uma chamando recursivamente
        //se alguma vencer do adversário que faz as melhores jogadas retornamos 1
        //se a melhor empatar com o adversário que faz as melhores jogadas retornamos 0
        //caso contrário retornamos -1
        vector<Action> actions = this->generate_actions(estado);

        vector<Estado> estados = {};

        for(int i=0;i<actions.size(); i++){

          //actions[i].jogador = this->inverter_jogador(actions[i].jogador);

          Estado estado2 = this->action_result(estado, actions[i]);
          estados.push_back(estado2);
        }

        vector<int> qualidades = {};

        for(int i = 0;i< actions.size();i++){
          qualidades.push_back(max(estados[i],this->inverter_jogador(jogador)));

          if(qualidades[i] == -1){
            break;}
        }

        int resultado = menor(qualidades);

        //salvo na lista memorizada
        memory.salvar_valor(resultado,estado);

        return resultado;
      }

    }






    //A IA quer maximizar custo()
    //retorna 1 se jogador consegue ganhar a partir do estado, 0 se ele empata e -1 se ele perde
    int max(Estado estado, char jogador){

      //antes de fazer as operações eu passo pela lista memorizada
      if(memory.ja_calculou(estado)){
        return memory.valor(estado);
      }

      if(this->final(estado)){
        //se estamos no final do jogo sabemos se a jogada que levou a esse estado é boa ou não
        //se X ganhou o custo é -1, se foi empate o custo é 0 e se X perdeu o custo é 1
        memory.salvar_valor(custo(estado),estado);
        return custo(estado);
      }
      else{
        //nesse caso geramos as jogadas possíveis e testamos cada uma chamando recursivamente
        //se alguma vencer do adversário que faz as melhores jogadas retornamos 1
        //se a melhor empatar com o adversário que faz as melhores jogadas retornamos 0
        //caso contrário retornamos -1
        vector<Action> actions = this->generate_actions(estado);


        vector<Estado> estados = {};

        for(int i=0;i<actions.size(); i++){

          //actions[i].jogador = this->inverter_jogador(actions[i].jogador);

          Estado estado2 = this->action_result(estado, actions[i]);

          estados.push_back(estado2);
        }

        vector<int> qualidades = {};

        for(int i = 0;i< actions.size();i++){
          qualidades.push_back(min(estados[i],this->inverter_jogador(jogador )));

          if(qualidades[i] == 1){break;}
        }

        int resultado = maior(qualidades);

        //salvo na lista memorizada
        memory.salvar_valor(resultado,estado);

        return resultado;

      }
    }


    Action jogada_computador(Estado estado){

      vector<Action> actions = this->generate_actions(estado);

      //cout<<"teste "<< actions[0].jogador<<endl;

      int melhor = 0;

      for(int i =0;i < actions.size();i++){
        Estado estado2 = this->action_result(estado,actions[i]);
        int qualidade = min(estado2,'X');

        if(qualidade == 1){
          return actions[i];
        }
        else if(qualidade == 0){
          melhor = i;
        }
      }
      return actions[melhor];
    }

    void jogada_computador2(Estado& estado){
      realizar_jogada(jogada_computador(estado), estado);
    }

};



int main(){

  while(true){

  Jogo_da_velha_modificado jogo1;

  //teste função max()

  /*
  jogo1.estado_atual.estado_atual_tabuleiro1 = {{'O','O','X'},{'X','O','O'},{'O','X','X'}};
  jogo1.estado_atual.estado_atual_tabuleiro2 = {{'O',' ','X'},{'X','O','O'},{'O','X',' '}};

  int a = jogo1.max(jogo1.estado_atual,'O');

  */

  int ganhador = 0;

  for(int i=0;i<9;i++){

      if(jogo1.final(jogo1.estado_atual)){
        cout<<"FIM DO JOGO"<<endl;
      }

      int turno = jogo1.get_turno(jogo1.estado_atual);

      if( (turno%2) == 0){
        cout<<"turno do computador: "<<endl;
        cout<<"turno: "<< turno <<endl;
        //jogo1.jogada_computador_random(jogo1.estado_atual);
        jogo1.jogada_computador2(jogo1.estado_atual);
        //jogo1.melhor_jogada(jogo1.estado_atual);
        jogo1.print();
        cout<<endl;
      }
      else{
        cout<<"turno do jogador: "<<endl;
        cout<<"turno: "<< turno <<endl;
        jogo1.jogada_jogador(jogo1.estado_atual);
        jogo1.print();
        cout<<endl;
      }

      ganhador = jogo1.custo(jogo1.estado_atual);
      if(ganhador != 0){
        break;
      }
  }


  if(jogo1.final(jogo1.estado_atual)){
    cout<<"FIM DO JOGO"<<endl;
  }

  if(ganhador == 1){
    cout<<"O ganhou"<<endl;
  }
  else if(ganhador==-1){
    cout<<"X ganhou"<<endl;
  }
  jogo1.print();

  }


  return 0;
}
