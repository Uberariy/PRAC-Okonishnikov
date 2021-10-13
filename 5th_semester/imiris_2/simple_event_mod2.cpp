#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <list>
#include <iostream>

using namespace std;

// Очень простой пример построения имитационной модели с календарём событий 
// Модель "самодостаточная" - не используются библиотеки для организации имитационного моделирования
// Возможности С++ используются недостаточно. Что можно улучшить в этой части?

class Event  // событие в календаре
{
public:
  float time; // время свершения события
  int type;   // тип события
  int attr; // дополнительные сведения о событии в зависимости от типа (время обработки)
  Event(float t, int tt, int a) {time = t; type = tt; attr = a;} 
};
// типы событий
#define EV_INIT 1 // начало обработки запроса
#define EV_REQ 2 // создание запроса клиентом
#define EV_FIN1 3 // окончание обработки запроса 1 процессора
#define EV_FIN2 4 // окончание обработки запроса 2 процессора
// состояния
#define RUN 1
#define IDLE 0

#define LIMIT 100 // время окончания моделирования

class Request // задание в очереди
{
public:
  float time;     // время выполнения задания без прерываний 
  int source_num; // номер источника заданий (1 или 2)
  Request(float t, int s) {time = t; source_num = s;} 
};  

class Calendar: public list<Event*> // календарь событий
{
  public:
  void put (Event* ev); // вставить событие в список с упорядочением по полю time
  Event* get (); // извлечь первое событие из календаря (с наименьшим модельным временем)
};

void Calendar::put(Event *ev)
{
  list<Event*>::iterator i;
  Event ** e = new (Event*);
  *e = ev;
  if( empty() ){ push_back(*e); return; }
  i = begin();
  while((i != end()) && ((*i)->time <= ev->time)) 
  {
    ++i;
  }
  insert(i, *e);
} 
Event* Calendar::get()
{  
  if(empty()) return NULL; 
  Event *e = front(); 
  pop_front();
  return e;
}


typedef list<Request*> Queue; // очередь заданий к процессору 

float get_req_time(int source_num); // длительность задания
float get_pause_time(int source_num); // длительность паузы между заданиями


int main(int argc, char **argv )
{

Calendar calendar;
Queue queue1;
Queue queue2;
float curr_time[2] = {0, 0};
Event *curr_ev;
float dt;
int cpu_state1 = IDLE;
int cpu_state2 = IDLE;
int proc_num = 0; // 0/1 для 1/2 процессора
float run_begin1;
float run_begin2; 
srand(2019);
// начальное событие и инициализация календаря
curr_ev = new Event(0, EV_INIT, 0);
calendar.put( curr_ev );
// цикл по событиям

while((curr_ev = calendar.get()) != NULL )
{
  cout << "Время события: " << curr_ev->time << ", тип: " << curr_ev->type << endl;
  curr_time[proc_num] = curr_ev->time; // продвигаем время
  // обработка события
  if ((curr_time[0] >= LIMIT) or (curr_time[1] >= LIMIT)) break; // типичное дополнительное условие останова моделирования


  switch(curr_ev->type)
  {
  case EV_INIT:  // запускаем генераторы запросов
    calendar.put(new Event(0, EV_REQ, 1));  
    calendar.put(new Event(0, EV_REQ, 2));  
    break;
  case EV_REQ:
    // планируем событие окончания обработки, если процессор свободен, иначе ставим в очередь
    dt = get_req_time(curr_ev->attr); 
	  cout << "dt " << dt << " num " << curr_ev->attr << endl;
    if (!proc_num) // 1 процессор
      if(cpu_state1 == IDLE) 
	  { 
	      cpu_state1 = RUN; 
	      calendar.put(new Event(curr_time[0]+dt, EV_FIN1, curr_ev->attr)); // планируем конец обработки
	      run_begin1 = curr_time[0]; // начинаем обработку
  	  } 
      else 
 	      queue1.push_back(new Request(dt, curr_ev->attr));  
    else          // 2 процессор
      if(cpu_state2 == IDLE) 
	  { 
	      cpu_state2 = RUN; 
	      calendar.put(new Event(curr_time[1]+dt, EV_FIN2, curr_ev->attr)); // планируем конец обработки
	      run_begin2 = curr_time[1]; // начинаем обработку
  	  } 
      else 
 	      queue2.push_back(new Request(dt, curr_ev->attr));  
    proc_num = (proc_num+1)%2;
    // планируем событие генерации следующего задания
    calendar.put(new Event(curr_time[proc_num]+get_pause_time(curr_ev->attr), EV_REQ, curr_ev->attr)); 
	  break;
  case EV_FIN1:
    curr_time[0] = curr_ev->time;
    // объявляем процессор свободным и размещаем задание из очереди, если таковое есть
    cpu_state1=IDLE; 
    // выводим запись о рабочем интервале
    cout << "Работа 1-ого процессора с " << run_begin1 << " по " << curr_ev->time << " длит. " << (curr_ev->time-run_begin1) << endl; 
    if (!queue1.empty()) 
    {
	    Request *rq = queue1.front(); 
	    queue1.pop_front(); 
	    calendar.put(new Event(curr_time[0]+rq->time, EV_FIN1, rq->source_num)); 
	    delete rq; 
	  } 
    run_begin1 = curr_time[0];
    break;
  case EV_FIN2:
    curr_time[1] = curr_ev->time;
    // объявляем процессор свободным и размещаем задание из очереди, если таковое есть
    cpu_state2=IDLE; 
    // выводим запись о рабочем интервале
    cout << "Работа 2-ого процессора с " << run_begin2 << " по " << curr_ev->time << " длит. " << (curr_ev->time-run_begin2) << endl; 
    if (!queue2.empty()) 
    {
	    Request *rq = queue2.front(); 
	    queue2.pop_front(); 
	    calendar.put(new Event(curr_time[1]+rq->time, EV_FIN2, rq->source_num)); 
	    delete rq; 
	  } 
    run_begin2 = curr_time[1];
    break;
  } // switch


   delete curr_ev;
} // while
} // main

int rc = 0; int pc = 0;
float get_req_time(int source_num)
{
  // Для демонстрационных целей - выдаётся случайное значение
  // при детализации модели функцию можно доработать
   double r = ((double)rand())/RAND_MAX;
   cout << "req " << rc << endl; rc++;
   if(source_num == 1) return r*10; else return r*20; 
}

float get_pause_time(int source_num) // длительность паузы между заданиями
{  
  // см. комментарий выше
   double p = ((double)rand())/RAND_MAX;
   cout << "pause " << pc << endl; pc++;
   if(source_num == 1) return p*20; else return p*10; 
}

