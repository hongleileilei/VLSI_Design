//ESE556 Project1
//Honglei Liu
//112848525


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//define node
struct Node{
  char *id;
  int partition;//represents which partition this node in    int gain;//gains
  int gain;
  int area;
  int lock;
  char *connections[10];
  struct Node* next;//used for linked list for gain list
};

struct Node_List{
  struct Node* start;
};

void List_init(struct Node_List *List){
  List->start = NULL;
}

void push(struct Node_List *List, char* id, int area, int partition){
    struct Node *p;
    p = malloc(sizeof(struct Node));
    p->id = (char*)malloc(sizeof(char*));
    strcpy(p->id,id);
    p->area = area;
    p->lock = 0;
    for(int conn_i=0;conn_i<10;conn_i++){
      p->connections[conn_i]=(char*)malloc(sizeof(char*));
    }
    p->partition = partition;
    p->next = List->start;
    List->start = p;
}

void add_connection(struct Node_List *List, char* src_name, char* dest_name){
  int added = 0;
  struct Node* current = List->start;
  while(current != NULL){
    if(strcmp(current->id,src_name)==0){
      for(int conn_i =0; conn_i<10; conn_i++){
        if(strcmp(current->connections[conn_i],"")==0){
          if(added == 0){
            strcpy(current->connections[conn_i],dest_name);
            added++;
          }
        }
      }
    }//find the source node
    current=current->next;
  }
}

void print_connections(struct Node_List *List, char* src_name){
  struct Node* current = List->start;
  while(current!= NULL){
    if(strcmp(current->id,src_name)==0){
      printf("connections nodes of %s is: ",current->id);
      for(int conn_i=0;conn_i<10;conn_i++){
        if(strcmp(current->connections[conn_i],"")!=0){
          printf("%s ",current->connections[conn_i]);
        }
      }
    }
    current = current->next;
  }
  printf("\n");
}

struct Node* pop(struct Node_List *List){
  struct Node* p;
  if(List->start != NULL){
    p = List->start;
    List->start = List->start->next;
    return p;
  }
  else{
    return NULL;
  }
}

void print_list(struct Node_List *List)
{
    struct Node *p = List->start;
    while(p != NULL) {
        printf("node %s(gain: %d) in partition %d has area %d", p->id,p->gain,p->partition,p->area);
        if(p->lock==0){
          printf(" and is free to move\n");
        }
        else{
          printf(" and is locked\n");
        }
        p = p->next;
    }
}

void print_table(char* table[50][50]){
  int i;
  int j;
  for(i=0;i<50;i++){
    for(j=0;j<50;j++){
      if(strcmp(table[i][j],"")!=0 && strcmp(table[i][j+1],"")!=0){
        printf("%s-> ",table[i][j]);
      }
      else{
        if(strcmp(table[i][j+1],"")==0){
          printf("%s",table[i][j]);
        }
      }
    }
    if(strcmp(table[i][0],"")==0){
      ;
    }
    else{
      printf("\n");
    }
  }
}

void Gain(struct Node_List* List, char* table[50][50], char* node_name){
  int FS=0;
  int TE=0;
  int G=0;
  int node_partition;
  int same_part = 0;
  int oppo_part = 0;
  struct Node* current = List->start;
  while(current != NULL){
    if(strcmp(current->id,node_name)==0){
      node_partition=current->partition;
    }
    current = current->next;
  }
  for(int table_row=0;table_row<50;table_row++){
    for(int table_col=0;table_col<50;table_col++){
      oppo_part=0;
      same_part=0;
      if(strcmp(table[table_row][table_col],"")!=0){
        if(strcmp(table[table_row][table_col],node_name)==0){//same row decide TE and FS
          for(int current_col=0;current_col<50;current_col++){
            if(strcmp(table[table_row][current_col],"")!=0 && current_col != table_col){
              struct Node* temp = List->start;
              while(temp != NULL){
                if(strcmp(table[table_row][current_col],temp->id)==0){
                //printf("%s(%d) is id in table, %s(%d) is node name\n",temp->id,temp->partition,node_name,node_partition);
                  if(temp->partition != node_partition){
                    oppo_part++;
                  }
                  else{
                    same_part++;
                  }
                }
                temp=temp->next;
              }
            }
          }
        }
        if(same_part==0){
          FS++;
        }
        if(oppo_part==0){
          TE++;
        }
      }
    }
  }
  G=FS-TE;
  current = List->start;
  while(current != NULL){
    if(strcmp(current->id,node_name)==0){
      current->gain = G;
      break;
    }
    current = current->next;
  }
}

void Gain_cal(struct Node_List* List, char* table[50][50]){
  struct Node* current = List->start;
  while(current != NULL){
    Gain(List, table, current->id);
    current = current->next;
  }
}

void swap(struct Node *a, struct Node *b)
{
  int temp_partition;
  int temp_gain;
  int temp_area;
  int temp_lock;
  char* temp_id = (char*)malloc(sizeof(char*));
  char* temp_conn[10];
  for(int t_c_i = 0; t_c_i<10; t_c_i++){
    temp_conn[t_c_i]=(char*)malloc(sizeof(char*));
  }
  temp_partition=a->partition;
  temp_gain=a->gain;
  temp_area=a->area;
  temp_lock=a->lock;
  strcpy(temp_id,a->id);
  for(int t_c_i=0;t_c_i<10;t_c_i++){
    strcpy(temp_conn[t_c_i],a->connections[t_c_i]);
  }
  a->partition = b->partition;
  a->gain = b->gain;
  a->area = b->area;
  a->lock = b->lock;
  strcpy(a->id,b->id);
  for(int t_c_i=0;t_c_i<10;t_c_i++){
    strcpy(a->connections[t_c_i],b->connections[t_c_i]);
  }
  b->partition=temp_partition;
  b->gain=temp_gain;
  b->area=temp_area;
  b->lock=temp_lock;
  strcpy(b->id,temp_id);
  for(int t_c_i=0;t_c_i<10;t_c_i++){
    strcpy(b->connections[t_c_i],temp_conn[t_c_i]);
  }
}
void bubbleSort_gain(struct Node *start)
{
  int swapped, i;
  struct Node *ptr1;
  struct Node *lptr = NULL;
  if (start == NULL)
      return;

  do
  {
      swapped = 0;
      ptr1 = start;

      while (ptr1->next != lptr)
      {
          if (ptr1->gain < ptr1->next->gain)
          {
              swap(ptr1, ptr1->next);
              swapped = 1;
          }
          ptr1 = ptr1->next;
      }
      lptr = ptr1;
  }
  while (swapped);
}
void bubbleSort_lock(struct Node *start)
{
  int swapped, i;
  struct Node *ptr1;
  struct Node *lptr = NULL;
  if (start == NULL)
      return;

  do
  {
      swapped = 0;
      ptr1 = start;

      while (ptr1->next != lptr)
      {
          if (ptr1->lock > ptr1->next->lock)
          {
              swap(ptr1, ptr1->next);
              swapped = 1;
          }
          ptr1 = ptr1->next;
      }
      lptr = ptr1;
  }
  while (swapped);
}

void sort_gain(struct Node_List* List){
  struct Node* current = List->start;
  bubbleSort_gain(current);
}
void sort_lock(struct Node_List* List){
  struct Node* current = List->start;
  bubbleSort_lock(current);
}
int all_locked(struct Node_List* List){
  struct Node* current = List->start;
  while(current != NULL){
    if(current->lock == 0){
      return 0;
    }
    current = current->next;
  }
  return 1;
}

void free_all_lock(struct Node_List* List){
  struct Node* current = List->start;
  while(current != NULL){
    current->lock = 0;
    current = current->next;
  }
}

int cut_size(struct Node_List* List){
  struct Node* current = List->start;
  struct Node* search = List->start;
  int result=0;
  while(current != NULL){
    for(int conn_i=0;conn_i<10;conn_i++){
      if(strcmp(current->connections[conn_i],"")!=0){
        search = List->start;
        while(search != NULL){
          if(strcmp(current->connections[conn_i],search->id)==0
          && search->partition != current->partition){
            result ++;
          }
          search = search->next;
        }
      }
    }
    current = current->next;
  }
  return result;
}

void FM_Algo_mods(struct Node_List* List,char* table[50][50], int amount){
  int ceiling = (int)ceil((double)0.7*amount);
  int flooring = (int)floor((double)0.3*amount);
  printf("module amount constraint is from %d to %d\n",flooring,ceiling);
  int amount_0=0;
  int amount_1=0;
  int amount_nodes=0;

  int min_cut;
  int cutsize=cut_size(List);
  min_cut=cutsize;
  int loop=0;
  int cycle = 0;
  int min_gain;
  char* min_part_0[7];
  char* min_part_1[7];
  for(int i=0; i<7; i++){
    min_part_0[i]=(char*)malloc(sizeof(char*));
    min_part_1[i]=(char*)malloc(sizeof(char*));
  }
  int min_part_0_i=0;
  int min_part_1_i=0;

  int new_min = 0;
  struct Node* current = List->start;
  while(current != NULL){
    if(current->partition==0){
      amount_0++;
    }
    else{
      amount_1++;
    }
    amount_nodes++;
    current = current->next;
  }
  current = List->start;
  while(cycle != amount_nodes){
    while(all_locked(List)!=1 && loop != amount_nodes){
      if(current->partition==0 && current->lock==0 && amount_0-1>=flooring && amount_1+1<=ceiling){
        loop = 0;
        current->partition=1;
        current->lock=1;
        cutsize = cut_size(List);
        amount_0--;
        amount_1++;
        //printf("%d is current cut size\n",cutsize);
        if(cutsize<min_cut){
          new_min = 1;
          min_part_0_i=0;
          min_part_1_i=0;
          min_gain=0;
          for(int i=0; i<7; i++){
            min_part_0[i] = (char*)realloc(min_part_0[i],sizeof(char*));
            min_part_1[i] = (char*)realloc(min_part_1[i],sizeof(char*));
          }
          min_cut=cutsize;
          struct Node* mem_temp = List->start;
          while(mem_temp != NULL){
            if(mem_temp->partition == 0){
              strcpy(min_part_0[min_part_0_i],mem_temp->id);
            }
            else{
              strcpy(min_part_1[min_part_1_i],mem_temp->id);
            }
            mem_temp = mem_temp->next;
          }
        }
        Gain_cal(List,table);
        if(new_min== 1){
          struct Node* mem_temp = List->start;
          while(mem_temp != NULL){
            min_gain += mem_temp->gain;
            mem_temp=mem_temp->next;
          }
          new_min=0;
        }
        sort_gain(List);
        current = List->start;
      }
      else if(current->partition==1 &&
          current->lock==0 &&
          amount_1-1>=flooring &&
          amount_0+1<=ceiling){
          loop = 0;
          current->partition=0;
          current->lock=1;
          cutsize = cut_size(List);
          amount_1--;
          amount_0++;
          //printf("%d is current cut size\n",cutsize);
          if(cutsize<min_cut){
            new_min = 1;
            min_part_0_i=0;
            min_part_1_i=0;
            min_gain=0;
            for(int i=0; i<7; i++){
              min_part_0[i] = (char*)realloc(min_part_0[i],sizeof(char*));
              min_part_1[i] = (char*)realloc(min_part_1[i],sizeof(char*));
            }
            min_cut=cutsize;
            struct Node* mem_temp = List->start;
            while(mem_temp != NULL){
              if(mem_temp->partition == 0){
                strcpy(min_part_0[min_part_0_i],mem_temp->id);
                min_part_0_i++;
              }
              else{
                strcpy(min_part_1[min_part_1_i],mem_temp->id);
                min_part_1_i++;
              }
              mem_temp = mem_temp->next;
            }
          }
          Gain_cal(List,table);
          if(new_min== 1){
            struct Node* mem_temp = List->start;
            while(mem_temp != NULL){
              min_gain += mem_temp->gain;
              mem_temp=mem_temp->next;
            }
            new_min=0;
          }
          sort_gain(List);
          current = List->start;
        }
        else{
          loop++;
          current = current->next;
          if(current == NULL){
            current = List->start;
          }
        }
        //print_list(List);
        //printf("\n");
      }
      cycle++;
      free_all_lock(List);
  }
  printf("%d is the minimum cut gains\n",min_gain);
  min_part_0_i=0;
  min_part_1_i=0;
  printf("min in partition 0: ");
  while(min_part_0_i<7 && strcmp(min_part_0[min_part_0_i],"")!=0){
    printf("%s ",min_part_0[min_part_0_i]);
    min_part_0_i++;
  }
  printf("\nmin in partition 1: ");
  while(min_part_1_i<7 && strcmp(min_part_1[min_part_1_i],"")!=0){
    printf("%s ",min_part_1[min_part_1_i]);
    min_part_1_i++;
  }
  printf("\nminimum cut size: %d\n",min_cut);
}

void FM_Algo_area(struct Node_List* List,char* table[50][50], int amount){
  int ceiling = (int)ceil((double)0.8*amount);
  int flooring = (int)floor((double)0.2*amount);
  printf("area constraint is from %d to %d\n",flooring,ceiling);
  int amount_0=0;
  int amount_1=0;
  int amount_nodes=0;

  int min_cut;
  int cutsize=cut_size(List);
  min_cut=cutsize;
  int loop=0;
  int cycle=0;
  int min_gain;
  char* min_part_0[7];
  char* min_part_1[7];
  for(int i=0; i<7; i++){
    min_part_0[i]=(char*)malloc(sizeof(char*));
    min_part_1[i]=(char*)malloc(sizeof(char*));
  }
  int min_part_0_i=0;
  int min_part_1_i=0;

  int new_min = 0;
  struct Node* current = List->start;
  while(current != NULL){
    if(current->partition==0){
      amount_0++;
    }
    else{
      amount_1++;
    }
    amount_nodes++;
    current = current->next;
  }
  current = List->start;
  while(cycle != amount_nodes){
    while(all_locked(List)!=1 && loop != amount_nodes){
      if(current->partition==0 && current->lock==0 && amount_0-current->area>flooring && amount_1+current->area<ceiling){
        loop = 0;
        current->partition=1;
        current->lock=1;
        cutsize = cut_size(List);
        amount_0 -= current->area;
        amount_1 += current->area;
        //printf("%d is current cut size\n",cutsize);
        if(cutsize<min_cut){
          new_min = 1;
          min_part_0_i=0;
          min_part_1_i=0;
          min_gain=0;
          for(int i=0; i<7; i++){
            min_part_0[i] = (char*)realloc(min_part_0[i],sizeof(char*));
            min_part_1[i] = (char*)realloc(min_part_1[i],sizeof(char*));
          }
          min_cut=cutsize;
          struct Node* mem_temp = List->start;
          while(mem_temp != NULL){
            if(mem_temp->partition == 0){
              strcpy(min_part_0[min_part_0_i],mem_temp->id);
            }
            else{
              strcpy(min_part_1[min_part_1_i],mem_temp->id);
            }
            mem_temp = mem_temp->next;
          }
        }
        Gain_cal(List,table);
        if(new_min== 1){
          struct Node* mem_temp = List->start;
          while(mem_temp != NULL){
            min_gain += mem_temp->gain;
            mem_temp=mem_temp->next;
          }
          new_min=0;
        }
        sort_gain(List);
        current = List->start;
      }
      else if(current->partition==1 &&
          current->lock==0 &&
          amount_1-current->area>flooring &&
          amount_0+current->area<ceiling){
          loop = 0;
          current->partition=0;
          current->lock=1;
          cutsize = cut_size(List);
          amount_1 -= current->area;
          amount_0 += current->area;
          //printf("%d is current cut size\n",cutsize);
          if(cutsize<min_cut){
            new_min = 1;
            min_part_0_i=0;
            min_part_1_i=0;
            min_gain=0;
            for(int i=0; i<7; i++){
              min_part_0[i] = (char*)realloc(min_part_0[i],sizeof(char*));
              min_part_1[i] = (char*)realloc(min_part_1[i],sizeof(char*));
            }
            min_cut=cutsize;
            struct Node* mem_temp = List->start;
            while(mem_temp != NULL){
              if(mem_temp->partition == 0){
                strcpy(min_part_0[min_part_0_i],mem_temp->id);
                min_part_0_i++;
              }
              else{
                strcpy(min_part_1[min_part_1_i],mem_temp->id);
                min_part_1_i++;
              }
              mem_temp = mem_temp->next;
            }
          }
          Gain_cal(List,table);
          if(new_min== 1){
            struct Node* mem_temp = List->start;
            while(mem_temp != NULL){
              min_gain += mem_temp->gain;
              mem_temp=mem_temp->next;
            }
            new_min=0;
          }
          sort_gain(List);
          current = List->start;
        }
        else{
          loop++;
          current = current->next;
          if(current == NULL){
            current = List->start;
          }
        }
        //print_list(List);
        //printf("\n");
      }
      cycle++;
      free_all_lock(List);
    }
    printf("%d is the minimum cut gains\n",min_gain);
    min_part_0_i=0;
    min_part_1_i=0;
    printf("min in partition 0: ");
    while(min_part_0_i<7 && strcmp(min_part_0[min_part_0_i],"")!=0){
      printf("%s ",min_part_0[min_part_0_i]);
      min_part_0_i++;
    }
    printf("\nmin in partition 1: ");
    while(min_part_1_i<7 && strcmp(min_part_1[min_part_1_i],"")!=0){
      printf("%s ",min_part_1[min_part_1_i]);
      min_part_1_i++;
    }
    printf("\nminimum cut size: %d\n",min_cut);
}

int main(){

  struct Node_List* cell_list;
  List_init(cell_list);
  printf("this implementation includes the consideration of pads!\n");
  FILE *fp_are;
  FILE *fp_net;
  char *line_are = NULL;
  char *line_net = NULL;
  size_t len_are = 0;
  size_t len_net = 0;
  ssize_t read_net;
  ssize_t read_are;
  char* filename_are = "/Users/liuhonglei/Desktop/spring2020/ESE556/try.are";
  char* filename_net = "/Users/liuhonglei/Desktop/spring2020/ESE556/try.net";
  fp_are = fopen(filename_are, "r");
  fp_net = fopen(filename_net, "r");
  if(fp_are == NULL){
    exit(EXIT_FAILURE);
  }
  char* nets_table[50][50];
  for(int row=0;row<50;row++){
    for(int col=0;col<50;col++){
      nets_table[row][col]=(char*)malloc(sizeof(char*));
    }
  }


  char *name=(char*)malloc(sizeof(char*));
  int area=0;
  int rand_part = 0;
  int node_arr_i = 0;
  int is_head = 1;
  int total_area=0;
  while((read_are = getline(&line_are, &len_are, fp_are) != -1)){
    name = strtok(line_are," ");
    area = atoi(strtok(NULL,""));
    total_area+=area;
    if(rand_part==0){
      push(cell_list,name,area,rand_part);
      rand_part=1;
    }
    else{
      push(cell_list,name,area,rand_part);
      rand_part=0;
    }
  }


  int line = 0;
  int num_pins = 0;
  int num_nets = 0;
  int num_mods = 0;
  char* src_name = (char*)malloc(sizeof(char*));
  char* src_name_hold = (char*)malloc(sizeof(char*));
  char* dest_name = (char*)malloc(sizeof(char*));
  int table_row = 0;
  int table_col = 0;
  int start = 0;
  while((read_net = getline(&line_net, &len_net, fp_net) != -1)){
    if(line == 0){
      printf("the first line is ignored\n");
    }
    else if(line == 1){
      num_pins = atoi(line_net);
      printf("# of pins: %d\n",num_pins);
    }
    else if(line == 2){
      num_nets = atoi(line_net);
      printf("# of nets: %d\n",num_nets);
    }
    else if(line == 3){
      num_mods = atoi(line_net);
      printf("# of mods: %d\n",num_mods);
    }
    else{//nets relationships
      if(strchr(line_net,'s')!=NULL){
        table_col=0;
        if(start == 0){
          src_name = strtok(line_net," ");
          strcpy(nets_table[table_row][table_col],src_name);
          strcpy(src_name_hold,src_name);
          table_col++;
          start = 1;
        }
        else{
          table_row++;
          src_name = strtok(line_net," ");
          strcpy(nets_table[table_row][table_col],src_name);
          strcpy(src_name_hold,src_name);
          table_col++;
        }

      }
      else{
        dest_name = strtok(line_net," ");
        add_connection(cell_list,src_name_hold,dest_name);
        strcpy(nets_table[table_row][table_col],dest_name);
        table_col++;
      }
    }
    line++;
  }
  Gain_cal(cell_list,nets_table);
  print_table(nets_table);
  print_list(cell_list);
  sort_gain(cell_list);
  printf("\n");
  printf("%d is initial cut size\n",cut_size(cell_list));
  print_list(cell_list);
  printf("\n");
  FM_Algo_mods(cell_list,nets_table,10);
  printf("\n");
  print_list(cell_list);
  printf("%d is final cut size\n",cut_size(cell_list));
  printf("\n");
  fclose(fp_are);
  fclose(fp_net);
  exit(EXIT_SUCCESS);
}
