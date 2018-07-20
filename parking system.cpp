#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>

#define MAX_SIZE 10

using namespace std;
//차 모델 1: 승용차, 2: 버스, 3: 트럭

class car {
  protected:
  int carNum;
  int carModel;
  time_t inTime;
  time_t outTime;
  int price;

  public:
  car (){

  }
  void init(int number, int model, time_t time, int init_price) {
    carNum = number;
    carModel = model;
    inTime = time;
    price = init_price;
  }
  int getCarnumber() {
    return carNum;
  }
  int getCarmodel() {
    return carModel;
  }
  int getPrice() {
    return price;
  }
  time_t getTime(){
    return inTime;
  }
  virtual int calcPrice(int init_price, time_t total_time){
    int price = 0;
  }
};

//차량종류에 따라 클래스를 만들어서 메소드를 상속받아 계산한다.
class sedan : public car{
  public:
    sedan (){

    }
    virtual int calcPrice(int init_price, time_t total_time){
      if (total_time % 10 != 0 ) total_time += 10;
      //요금계산
      price = init_price + (int)(total_time/10)*1000;
      return price;
    }

};
class bus : public car{
  public:
    bus (){

    }
    virtual int calcPrice(int init_price, time_t total_time){
      if (total_time % 10 != 0 ) total_time += 10;
      //요금계산
      price = init_price + (int)(total_time/10)*1500;
      return price;
    }
};
class truck : public car{
  public:
    truck (){

    }
    virtual int calcPrice(int init_price, time_t total_time){
      if (total_time % 10 != 0 ) total_time += 10;
      //요금계산
      price = init_price + (int)(total_time/10)*3000;
      return price;
    }
};

class parkingLot {
  vector<car *> carList;
  int totalMoney = 0;
  int capacity = 0;

  public:
  void car_init (int num, int model){
    int price;
    time_t intime;
    do{

      //입차한 시간에 따라 시간 자동 반영 (현재시간저장)
      intime = time(0);
      //if문으로 차량 종류에 따라 기본요금설정
      if(model==1) {
        price = 1000;
        //입력한 정보바탕으로 차량 객체 생성
        sedan* sedans = new sedan();
        sedans -> init(num,model,intime,price);
        //car list 벡터에 넣어줌
        carList.push_back(sedans);
        //capacity를 차량 종류에 따라 늘림
        capacity++;
      }
      else if(model==2) {
        price = 3000;
        //입력한 정보바탕으로 차량 객체 생성
        bus* buses = new bus();
        buses -> init(num,model,intime,price);
        //car list 벡터에 넣어줌
        carList.push_back(buses);
        //capacity를 차량 종류에 따라 늘림
        capacity+=2;
      }
      else if(model==3) {
        price = 5000;
        //입력한 정보바탕으로 차량 객체 생성
        truck* trucks = new truck();
        trucks -> init(num,model,intime,price);
        //car list 벡터에 넣어줌
        carList.push_back(trucks);
        //capacity를 차량 종류에 따라 늘림
        capacity+=3;
      }
    }while(model != 1 && model != 2 && model != 3 );

    cout << "입차가 완료되었습니다." <<endl;
  }

  bool isFull (int model){
    if (capacity+model > MAX_SIZE) return true;
    else return false;
  }

  void car_out (int& carNumber){ // 출차

    time_t in_time, total_time;
    time_t out_time=time(0);
    int init_price, price, model;
    for(int i=0;i<carList.size();i++){
      if(carList[i]->getCarnumber() == carNumber){
        in_time = carList[i]->getTime();
        init_price = carList[i]->getPrice();
        model = carList[i]->getCarmodel();
        total_time = out_time - in_time;
        price = carList[i]->calcPrice(init_price,total_time);
        totalMoney += price;
        //car list에서 제외시킴
        carList.erase(carList.begin()+i);
        if(model == 1 ) capacity--;
        else if(model == 2) capacity-=2;
        else if(model == 3) capacity-=3;
      }
    }

    cout << "총 주차시간은" << total_time << "초입니다." <<endl;
    cout << "출차요금은 " << price << "원입니다." <<endl;
    cout << "출차가 완료되었습니다." << endl;
  }

  void showcarList(){
    string model;
    //벡터에 저장된 정보를 출력
     for(int i=0;i<carList.size();i++){
      cout << i+1 <<"번째 주차된 차량 정보"<<endl;
      cout << "차량번호 : " << carList[i]->getCarnumber() <<endl;
      if(carList[i]->getCarmodel() == 1) model = "승용차";
      else if(carList[i]->getCarmodel() == 2) model ="버스";
      else if(carList[i]->getCarmodel() == 3) model ="트럭";
      cout << "차량종류 : " << model <<endl;
      cout << "입차시간 : " << timeToString(carList[i]->getTime()) << endl <<endl;
    }
  }

  void showcarshortlist(){
    cout << "주차된 차량 목록"<<endl;
    for(int i=0;i<carList.size();i++){
      cout << "차량번호 : " << carList[i]->getCarnumber() <<endl;
    }
  }

  void totalprice(){
    //출차시 계산한 가격 합산
    cout << "총 합산 가격은 " << totalMoney << "원 입니다." <<endl;
  }

  void print(){
    ofstream outFile("output.txt");
    outFile << "현재 주차 차량 목록"<<endl;
    for(int i=0;i<carList.size();i++){
      outFile << "차량번호 : " << carList[i]->getCarnumber() <<endl;
    }
    outFile << "오늘 총 매출은 " << totalMoney << "원 입니다."<<endl;
    outFile.close();
  }

  string timeToString(time_t t)
  {
  struct tm * tstruct;
  char str[80];
  tstruct = localtime(&t);
  strftime(str, sizeof(str), "[%Y-%m-%d] %X", tstruct);
  return str;
  }
};

int main() {
  int key;
  parkingLot parking;

  while(1)
	{
		cout << "\n1: 입차 2: 출차 3: 주차차량 보기 4: 총 수입계산 5: 보고서" << endl;
		cout << "위 메뉴 중에서 하나를 선택하시오.(0:종료) ------> ";
		cin >> key;
    int num=0, model=0;

		switch(key){
			case 0:
				cout << "종료합니다.\n";
				return 0;

			case 1:

        do{
          cout << "차량번호를 입력하세요.(1000~9999)" << endl;
          cin >> num;
        } while(num < 1000 || num > 9999);

        cout << "차량 종류를 선택하세요 (1. 승용차 2. 버스 3. 트럭)"<<endl;
        cin >> model;

        if(parking.isFull(model)) {
          cout << "주차공간이 없습니다." <<endl;
          break;
        }else	parking.car_init(num,model);
				break;

			case 2:
			  parking.showcarshortlist();
			  int num;
			  cout << endl;
			  cout << "출차할 차량번호를 입력하세요." << endl;
			  cin >> num;
				parking.car_out(num);
				break;
			case 3:
				parking.showcarList();
				break;
			case 4:
				parking.totalprice();
				break;
			case 5:
        parking.print();
        cout << "보고서가 출력되었습니다." <<endl;
			  break;
			default:
				cout << "잘못 입력했습니다. 다시 입력하시오\n";
				break;
		}
	}
	cout << "종료되었습니다.";
	return 0;
}
