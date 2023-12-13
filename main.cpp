#include <bits/stdc++.h>
using namespace std;

#define QTD_VIZINHANCAS 3

struct Lines{
  vector<int> products; //vetor com os índices dos produtos
  int time; //tempo total de cada linha
};

int nLines;
int nProducts;
int *productTime;
int **matrix;

void readFile(int* nLines, int* nProducts, int** productTime, int*** matrix) { //leitura do arquivo
  int numberOfLines;
  int numberOfProducts;
  int *timeOfProduct;
  int **timeMatrix;
  
  ifstream arquivo("arquivo.txt");

  if(!arquivo.is_open()) {
    cout << "Não foi possível abrir o arquivo!";
    arquivo.close();
    exit(0);
  }

  arquivo >> numberOfLines >> numberOfProducts;
  timeOfProduct = new int[numberOfProducts];
  
  for(int j = 0; j < numberOfProducts; j++) { 
    arquivo >> timeOfProduct[j];
   }
  
  timeMatrix = new int*[numberOfProducts];
  
  for(int i = 0; i < numberOfProducts; i++) {
    timeMatrix[i] = new int[numberOfProducts];
    
    for(int j = 0; j < numberOfProducts; j++) {
      arquivo >> timeMatrix[i][j];
    }
  }

  *nLines = numberOfLines;
  *nProducts = numberOfProducts;
  *productTime = timeOfProduct;
  *matrix = timeMatrix;

  delete timeOfProduct;
  delete timeMatrix;
} 

int guloso(int *productTime, Lines* lines) { //algoritmo guloso
  int minTime;
  int lineTime;
  int chosenLine;
  int maxTime = 0;

  for(int i = 0; i < nLines; i++) { //preenche o primeiro produto de cada linha
    lines[i].products.push_back(i);
    lines[i].time = productTime[i];
    
    if(lines[i].time > maxTime)
      maxTime = lines[i].time;
  }

  for(int i = nLines; i < nProducts; i++) {
    minTime = lines[0].time + productTime[i] + matrix[lines[0].products.back()][i];
    chosenLine = 0;
      
    for(int j = 1; j < nLines; j++) {
      lineTime = lines[j].time + productTime[i] + matrix[lines[j].products.back()][i];
        
      if(lineTime < minTime) {
        minTime = lineTime;
        chosenLine = j;
      }
    }

    lines[chosenLine].products.push_back(i);
    lines[chosenLine].time = minTime;

    if(minTime > maxTime)
      maxTime = minTime;
  }

  return maxTime;
}

int swapVertical(int *productTime, Lines* lines, int maxTime){
  int swapTimeMax, swapTime;
  int chosenMaxLine;
  int aux;
  int best_i = -1, best_j = -1, bestLine = -1;
  int sMax = maxTime;
  int s, time_h;
  

  //escolhendo a pior linha (maior tempo)
  for(int i = 0; i < nLines; i++) {
    if(lines[i].time == maxTime) {
      chosenMaxLine = i;
      break;
    }
  }

  for(int h = 0; h < nLines; h++){//iterar nas linhas
    s = lines[h].time;
    
    if(h == chosenMaxLine)
      continue;
    //lines[chosenMaxLine].products[j]
    for(int i = 0; i < lines[chosenMaxLine].products.size(); i++){ //iterar entre os produtos na linha com o tempo maior
      //lines[chosenMaxLine].products[k]
      for(int j = 0; j < lines[h].products.size(); j++){//iterar entre os produtos na linha i
        swapTimeMax = sMax;
        swapTime = s;

        swapTimeMax -= productTime[lines[chosenMaxLine].products[i]];
        swapTimeMax += productTime[lines[h].products[j]];
        swapTime -= productTime[lines[h].products[j]];
        swapTime += productTime[lines[chosenMaxLine].products[i]];

        if(i == 0){
          swapTimeMax -= matrix[lines[chosenMaxLine].products[i]][lines[chosenMaxLine].products[i+1]];
          swapTimeMax += matrix[lines[h].products[j]][lines[chosenMaxLine].products[i+1]];

        }else if(i == lines[chosenMaxLine].products.size()-1){
          swapTimeMax -= matrix[lines[chosenMaxLine].products[i-1]][lines[chosenMaxLine].products[i]];
          swapTimeMax += matrix[lines[chosenMaxLine].products[i-1]][lines[h].products[j]];
          
        }else{
          swapTimeMax -= matrix[lines[chosenMaxLine].products[i]][lines[chosenMaxLine].products[i+1]];
          swapTimeMax += matrix[lines[h].products[j]][lines[chosenMaxLine].products[i+1]];
          swapTimeMax -= matrix[lines[chosenMaxLine].products[i-1]][lines[chosenMaxLine].products[i]];
          swapTimeMax += matrix[lines[chosenMaxLine].products[i-1]][lines[h].products[j]];
        }

        if(j == 0){
          swapTime -= matrix[lines[h].products[j]][lines[h].products[j+1]];
          swapTime += matrix[lines[chosenMaxLine].products[i]][lines[h].products[j+1]];
          
        }else if(j == lines[h].products.size() - 1){
          swapTime -= matrix[lines[h].products[j-1]][lines[h].products[j]];
          swapTime += matrix[lines[h].products[j-1]][lines[chosenMaxLine].products[i]];
          
        }else{
          swapTime -= matrix[lines[h].products[j]][lines[h].products[j+1]];
          swapTime += matrix[lines[chosenMaxLine].products[i]][lines[h].products[j+1]];
          swapTime -= matrix[lines[h].products[j-1]][lines[h].products[j]];
          swapTime += matrix[lines[h].products[j-1]][lines[chosenMaxLine].products[i]];
        }

        if(swapTime < maxTime && swapTimeMax < maxTime) {
          maxTime = swapTimeMax;
          time_h = swapTime;
          best_i = i;
          best_j = j;
          bestLine = h;
        }
        
      }
    }
  }

  if(best_i >= 0 && best_j >= 0 && bestLine >= 0){
    aux = lines[bestLine].products[best_j];
    lines[bestLine].products[best_j] = lines[chosenMaxLine].products[best_i];
    lines[chosenMaxLine].products[best_i] = aux;
    lines[bestLine].time = time_h;
  }
  
  lines[chosenMaxLine].time = maxTime;
  
  for(int i = 0; i < nLines; i++) {
    if(lines[i].time > maxTime) {
      maxTime = lines[i].time;
    }
  }
  
  return maxTime;
}

int swapHorizontal(Lines* lines, int maxTime) { //primeiro movimento de vizinhança (swap)
  int swapTime;
  int chosenLine;
  int aux;
  int best_i = 0, best_j = 0;
  int s = maxTime;
  

  for(int i = 0; i < nLines; i++) {
    if(lines[i].time == maxTime) {
      chosenLine = i;
      break;
    }
  }

  for(int i = 0; i < lines[chosenLine].products.size()-1; i++) {
    for(int j = i + 1; j < lines[chosenLine].products.size(); j++) {
      swapTime = s;
      //retirar a aresta depois do i
      swapTime -= matrix[lines[chosenLine].products[i]][lines[chosenLine].products[i+1]];       //sempre tira essa aresta

      if(j != i + 1){//não adjacentes
        swapTime -= matrix[lines[chosenLine].products[j-1]][lines[chosenLine].products[j]];
        swapTime += matrix[lines[chosenLine].products[j]][lines[chosenLine].products[i+1]];
        swapTime += matrix[lines[chosenLine].products[j-1]][lines[chosenLine].products[i]];
      }else{//adjacentes
        swapTime += matrix[lines[chosenLine].products[j]][lines[chosenLine].products[i]];
      }

      if(i > 0){
        swapTime -= matrix[lines[chosenLine].products[i-1]][lines[chosenLine].products[i]];
        swapTime += matrix[lines[chosenLine].products[i-1]][lines[chosenLine].products[j]];
      }

      if(j < lines[chosenLine].products.size() - 1){
        swapTime -= matrix[lines[chosenLine].products[j]][lines[chosenLine].products[j+1]];
        swapTime += matrix[lines[chosenLine].products[i]][lines[chosenLine].products[j+1]];
      }

      if(swapTime < maxTime) {
        maxTime = swapTime;
        best_i = i;
        best_j = j;
      }
      
    }
  }

  aux = lines[chosenLine].products[best_j];
  lines[chosenLine].products[best_j] = lines[chosenLine].products[best_i];
  lines[chosenLine].products[best_i] = aux;
  
  lines[chosenLine].time = maxTime;
  
  for(int i = 0; i < nLines; i++) {
      if(lines[i].time > maxTime) {
        maxTime = lines[i].time;
        }
    }
    
    return maxTime;
}

int reinsertion(int *productTime, Lines* lines, int maxTime){
  int reinsertionTime;
  int chosenLine;
  int aux, best_i = 0, best_j = 0;
  int s = maxTime;
  
  for(int i = 0; i < nLines; i++){
    if(lines[i].time == maxTime){
      chosenLine = i;
      break;
    }
  }
  
  for(int i = 0; i < lines[chosenLine].products.size()-2; i++){ //tirar o products.size() porque provavelmente é O(n), passar o tamanho do vetor
    for(int j = i+2; j < lines[chosenLine].products.size(); j++){

      reinsertionTime = s;
      
      reinsertionTime -= matrix[lines[chosenLine].products[i]][lines[chosenLine].products[i+1]];
      reinsertionTime += matrix[lines[chosenLine].products[j]][lines[chosenLine].products[i]];

      if(i > 0){
        reinsertionTime -= matrix[lines[chosenLine].products[i-1]][lines[chosenLine].products[i]];
        reinsertionTime += matrix[lines[chosenLine].products[i-1]][lines[chosenLine].products[i+1]];
      }

      if(j < lines[chosenLine].products.size()-1){
        reinsertionTime -= matrix[lines[chosenLine].products[j]][lines[chosenLine].products[j+1]];
        reinsertionTime += matrix[lines[chosenLine].products[i]][lines[chosenLine].products[j+1]];
      }

      if(reinsertionTime < maxTime){
        maxTime = reinsertionTime;
        best_i = i;
        best_j = j;
      }
    }
  
  }
  
  int produto = lines[chosenLine].products[best_i];

  lines[chosenLine].time = maxTime;
  lines[chosenLine].products.insert(lines[chosenLine].products.begin() + best_j+1, produto);
  lines[chosenLine].products.erase(lines[chosenLine].products.begin() + best_i);

  for(int i = 0; i < nLines; i++) {
      if(lines[i].time > maxTime) {
        maxTime = lines[i].time;
        }
    }
  
  return maxTime;
}

int VND(int r, int *productTime, Lines* lines, int maxTime){
  int k = 1;
  int s = maxTime;
  
  while(k <= r){
    switch(k){
      case 1:
        maxTime = swapHorizontal(lines, maxTime);
        break;
      
      case 2:
        maxTime = reinsertion(productTime, lines, maxTime);
        break;

      case 3:
        maxTime = swapVertical(productTime, lines, maxTime);
        break;
    }
    
    

    if(maxTime < s){
      s = maxTime;
      k = 1;
    }else{
      k++;
      maxTime = s;
    }
    
  }

  return s;
}

int perturbacao(Lines* copia, int maxTime){
  int i, j;
  int chosenMaxLine = 0;
  int aux;

  for(int i = 0; i < nLines; i++) {
    if(copia[i].time == maxTime) {
      chosenMaxLine = i;
      break;
    }
  }

  int h = chosenMaxLine;
  while(h == chosenMaxLine){
    h = rand() % nLines;
  }
  
  i = rand() % copia[chosenMaxLine].products.size();
  j = rand() % copia[h].products.size();

  int swapTimeMax = copia[chosenMaxLine].time;
  int swapTime = copia[h].time;

  swapTimeMax -= productTime[copia[chosenMaxLine].products[i]];
  swapTimeMax += productTime[copia[h].products[j]];
  swapTime -= productTime[copia[h].products[j]];
  swapTime += productTime[copia[chosenMaxLine].products[i]];

  if(i == 0){
    swapTimeMax -= matrix[copia[chosenMaxLine].products[i]][copia[chosenMaxLine].products[i+1]];
    swapTimeMax += matrix[copia[h].products[j]][copia[chosenMaxLine].products[i+1]];

  }else if(i == copia[chosenMaxLine].products.size()-1){
    swapTimeMax -= matrix[copia[chosenMaxLine].products[i-1]][copia[chosenMaxLine].products[i]];
    swapTimeMax += matrix[copia[chosenMaxLine].products[i-1]][copia[h].products[j]];
          
  }else{
    swapTimeMax -= matrix[copia[chosenMaxLine].products[i]][copia[chosenMaxLine].products[i+1]];
    swapTimeMax += matrix[copia[h].products[j]][copia[chosenMaxLine].products[i+1]];
    swapTimeMax -= matrix[copia[chosenMaxLine].products[i-1]][copia[chosenMaxLine].products[i]];
    swapTimeMax += matrix[copia[chosenMaxLine].products[i-1]][copia[h].products[j]];
  }

  if(j == 0){
    swapTime -= matrix[copia[h].products[j]][copia[h].products[j+1]];
    swapTime += matrix[copia[chosenMaxLine].products[i]][copia[h].products[j+1]];
          
  }else if(j == copia[h].products.size() - 1){
    swapTime -= matrix[copia[h].products[j-1]][copia[h].products[j]];
    swapTime += matrix[copia[h].products[j-1]][copia[chosenMaxLine].products[i]];
          
  }else{
    swapTime -= matrix[copia[h].products[j]][copia[h].products[j+1]];
    swapTime += matrix[copia[chosenMaxLine].products[i]][copia[h].products[j+1]];
    swapTime -= matrix[copia[h].products[j-1]][copia[h].products[j]];
    swapTime += matrix[copia[h].products[j-1]][copia[chosenMaxLine].products[i]];
  }

  aux = copia[h].products[j];
  copia[h].products[j] = copia[chosenMaxLine].products[i];
  copia[chosenMaxLine].products[i] = aux;
  copia[h].time = swapTime;
  copia[chosenMaxLine].time = swapTimeMax;

  for(int i = 0; i < nLines; i++) {
      if(copia[i].time > maxTime) {
        maxTime = copia[i].time;
        }
    }
  
  return maxTime;
}

void copiaSolucao(Lines* lines, Lines* copia){
  for(int i = 0; i < nLines; i++){
    copia[i].time = lines[i].time;
    for(int j = 0; j < lines[i].products.size(); j++){
      copia[i].products.push_back(lines[i].products[j]);
    }
  }

}

int ILS(int *productTime, Lines* lines, Lines* copia){
  int s_ILS;
  int sInicial;
  int sVND;
  int sPerturbacao;
  int contador = 0;
  int naoMelhorou = 0;
  
  auto start = chrono::steady_clock::now();
  cout << "Calculando solução inicial..." << endl;
  sInicial = guloso(productTime, lines);
  auto end = chrono::steady_clock::now();

  chrono::duration<double> elapsed_seconds = end - start;

  cout << "Solução do guloso: " << sInicial << endl;
  cout << "Tempo de execução(Heuristica de construção): " << elapsed_seconds.count() << endl<< endl;

  

  auto start2 = chrono::steady_clock::now();
  cout << "Fazendo busca local..." << endl;
  sVND = VND(QTD_VIZINHANCAS, productTime, lines, sInicial);
  auto end2 = chrono::steady_clock::now();

  chrono::duration<double> elapsed_seconds2 = end2 - start2;

  cout << "Solução do VND: " << sVND << endl;
  cout << "Tempo de execução(VND): " <<  elapsed_seconds2.count() << endl;

  copiaSolucao(lines, copia);
  
  while(contador < 100 && naoMelhorou < 7){
    sPerturbacao = perturbacao(copia, sVND);
    s_ILS = VND(QTD_VIZINHANCAS, productTime, copia, sPerturbacao);

    if(s_ILS < sVND){
      sVND = s_ILS;
      naoMelhorou = 0;
      
    }else{
      naoMelhorou++;
    }
    
    contador++;
  }

  return sVND;
}

int main() {
  int maxTime;
  int solution;

  srand(time(NULL));

  readFile(&nLines, &nProducts, &productTime, &matrix);

  Lines lines[nLines];
  Lines copia[nLines];

  auto start = chrono::steady_clock::now();

  solution = ILS(productTime, lines, copia);

  auto end = chrono::steady_clock::now();

  chrono::duration<double> elapsed_seconds = end - start;

  cout << "Valor da solução após o ILS: " << solution << "\n";
  cout << "Tempo de execução(ILS): " << elapsed_seconds.count() << endl;

  return 0;
}