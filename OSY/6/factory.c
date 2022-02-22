/*
 * @author: timusfed
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
 
#define PROD_SLOT 0
#define STAGE_SLOT 1
 
enum place
{
    NUZKY,
    VRTACKA,
    OHYBACKA,
    SVARECKA,
    LAKOVNA,
    SROUBOVAK,
    FREZA,
    _PLACE_COUNT
};
 
const char *place_str[_PLACE_COUNT] = {
    [NUZKY] = "nuzky",
    [VRTACKA] = "vrtacka",
    [OHYBACKA] = "ohybacka",
    [SVARECKA] = "svarecka",
    [LAKOVNA] = "lakovna",
    [SROUBOVAK] = "sroubovak",
    [FREZA] = "freza",
};
 
enum product
{
    A,
    B,
    C,
    _PRODUCT_COUNT
};
 
const char *product_str[_PRODUCT_COUNT] = {
    [A] = "A",
    [B] = "B",
    [C] = "C",
};
 
enum status
{
    NOT_BUSY,
    BUSY,
    IS_WORKING
};
 
typedef struct Worker
{
    char *name;
    char *workplace;
    int workplace_id;
    int status;
    int counter;
} Worker_t;
 
#define _PHASE_COUNT 6
pthread_mutex_t stages_m = PTHREAD_MUTEX_INITIALIZER;
int stages[_PRODUCT_COUNT][_PHASE_COUNT] = {
    [A] = {0, 0, 0, 0, 0, 0},
    [B] = {0, 0, 0, 0, 0, 0},
    [C] = {0, 0, 0, 0, 0, 0}};
 
#define INIT_SIZE 32
pthread_mutex_t workers_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t workers_end_m = PTHREAD_MUTEX_INITIALIZER;
pthread_t *worker_id_list;
int workers_count;
int threads_count;
Worker_t **worker_list;
char **end_worker_list;
int worker_list_size;
 
pthread_mutex_t place_m[_PLACE_COUNT];
int ready_places_list[_PLACE_COUNT];
 
pthread_mutex_t end_place_m[_PLACE_COUNT];
int remove_place_list[_PLACE_COUNT];
 
sem_t sem_workplace[_PLACE_COUNT];
sem_t sem_product[_PLACE_COUNT];
 
sem_t *sem_exit;
 
sem_t *shift_end;
 
int find_string_in_array(const char **array, int length, char *what);
int placeByStage(int product, int stage);
void *startWorker(void *arg);
int removePlaceCheck(int workplace_id);
void definePurpose(int *data, int workplace_id);
void removeWorker(Worker_t *worker);
int checkEndList(char *name);
void setStatus(Worker_t *worker, int status);
int getStatus(Worker_t *worker);
void changeStage(int product, int stage, char op);
void changePlaces(int workplace_id, char op);
int hasFreePlace(int workplace_id);
void changeEndPlaces(int workplace_id, char op);
float howLongWait(int workplace_id);
char fasterProductStr(int product);
void endWorker(char *name);
 
int hasJob(Worker_t *worker);
Worker_t *findWorkerByPlace(int workplace_id);
int jobInStagesBefore(int p, int s);
int noActiveWorkers(char *sender_name);
 
pthread_mutex_t print_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t global_m = PTHREAD_MUTEX_INITIALIZER;
int global_end = 0;
 
int main(int argc, char **argv)
{
    // init mutex & semaphore
    for (int i = 0; i < _PLACE_COUNT; i++)
    {
        pthread_mutex_init(&place_m[i], NULL);
        pthread_mutex_init(&end_place_m[i], NULL);
        sem_init(&sem_workplace[i], 0, 0);
        sem_init(&sem_product[i], 0, 0);
    }
 
    // init worker list
    worker_list_size = INIT_SIZE;
    workers_count = 0;
    worker_list = calloc(worker_list_size, sizeof(Worker_t *));
    end_worker_list = calloc(worker_list_size, sizeof(char *));
    sem_exit = calloc(worker_list_size, sizeof(sem_t));
    shift_end = calloc(worker_list_size, sizeof(sem_t));
 
    worker_id_list = calloc(worker_list_size, sizeof(pthread_t));
    threads_count = 0;
 
    while (1)
    {
        char *line, *cmd, *arg1, *arg2, *arg3, *saveptr;
        int s = scanf(" %m[^\n]", &line);
        if (s == EOF)
            break;
        if (s == 0)
            continue;
 
        cmd = strtok_r(line, " ", &saveptr);
        arg1 = strtok_r(NULL, " ", &saveptr);
        arg2 = strtok_r(NULL, " ", &saveptr);
        arg3 = strtok_r(NULL, " ", &saveptr);
 
        if (strcmp(cmd, "start") == 0 && arg1 && arg2 && !arg3)
        {
            /* - start new thread for new worker
             * - copy (e.g. strdup()) worker name from arg1, the
             *   arg1 will be removed at the end of scanf cycle
             * - workers should have dynamic objects, you don't know
             *   total number of workers
             */
 
            // define new worker
            Worker_t *worker = malloc(sizeof(Worker_t));
            worker->name = strdup(arg1);
            worker->workplace = strdup(arg2);
            worker->workplace_id = find_string_in_array(place_str, _PLACE_COUNT, worker->workplace);
            worker->status = NOT_BUSY;
            worker->counter = workers_count;
 
            sem_init(&sem_exit[workers_count], 0, 0);
            sem_init(&shift_end[workers_count], 0, 0);
 
            if (worker->workplace_id != -1)
            {
 
                // create new worker thread, save his name in
                for (int i = 0; i < worker_list_size; i++)
                {
                    if (worker_list[i] == NULL)
                    {
                        worker_list[i] = worker;
                        break;
                    }
                }
 
                pthread_create(&worker_id_list[threads_count++], NULL, startWorker, (void *)worker);
 
                if (threads_count >= worker_list_size)
                {
                    // realloc
                    worker_list_size += INIT_SIZE;
                    worker_id_list = realloc(worker_id_list, worker_list_size * sizeof(pthread_t));
                    worker_list = realloc(worker_list, worker_list_size * sizeof(Worker_t *));
                    end_worker_list = realloc(end_worker_list, worker_list_size * sizeof(char *));
                    sem_exit = realloc(sem_exit, worker_list_size * sizeof(sem_t));
                    shift_end = realloc(shift_end, worker_list_size * sizeof(sem_t));
                }
                workers_count += 1;
            }
            else
            {
                free(worker->name);
                free(worker->workplace);
                free(worker);
            }
        }
        else if (strcmp(cmd, "make") == 0 && arg1 && !arg2)
        {
            int product = find_string_in_array(product_str, _PRODUCT_COUNT, arg1);
            if (product >= 0)
            {
                changeStage(product, 0, '+');
                sem_post(&sem_product[placeByStage(product, 0)]);
            }
        }
        else if (strcmp(cmd, "end") == 0 && arg1 && !arg2)
        {
            char *name = strdup(arg1);
            endWorker(name);
            free(name);
        }
        else if (strcmp(cmd, "add") == 0 && arg1 && !arg2)
        {
            char *place = strdup(arg1);
            int workplace_id = find_string_in_array(place_str, _PLACE_COUNT, place);
            if (workplace_id != -1)
            {
                changePlaces(workplace_id, '+');
                sem_post(&sem_workplace[workplace_id]);
            }
            free(place);
        }
        else if (strcmp(cmd, "remove") == 0 && arg1 && !arg2)
        {
            char *place = strdup(arg1);
            int workplace_id = find_string_in_array(place_str, _PLACE_COUNT, place);
            if (workplace_id != -1)
            {
                if (hasFreePlace(workplace_id))
                {
                    // remove place by main thread if it's not busy
                    changePlaces(workplace_id, '-');
                }
                else
                {
                    // else signal to worker to remove it
                    changeEndPlaces(workplace_id, '+');
                    sem_post(&sem_product[workplace_id]);
                }
            }
            free(place);
        }
        else
        {
            fprintf(stderr, "Invalid command: %s\n", line);
        }
        free(line);
    }
 
    /* Wait for every worker to finish their work. Nobody should be able to
     * continue.
     */
    pthread_mutex_lock(&global_m);
    global_end = 1;
    pthread_mutex_unlock(&global_m);
 
    for (int i = 0; i < _PLACE_COUNT; i++)
    {
        sem_post(&sem_workplace[i]);
        sem_post(&sem_product[i]);
    }
 
    // wait for all threads to end
    for (int i = 0; i < threads_count; i++)
    {
        if (worker_id_list[i])
        {
            pthread_join(worker_id_list[i], NULL);
        }
    }
}
 
void endWorker(char *name)
{
    // find the name needed
    pthread_mutex_lock(&workers_m);
    for (int i = 0; i < worker_list_size; i++)
    {
        if (worker_list[i])
        {
            if (strcmp(name, worker_list[i]->name) == 0)
            {
 
                // put the name into remove list
                for (int j = 0; j < worker_list_size; j++)
                {
                    if (end_worker_list[j] == NULL)
                    {
                        // send signals
                        end_worker_list[j] = worker_list[i]->name;
                        sem_post(&sem_workplace[worker_list[i]->workplace_id]);
                        sem_post(&sem_product[worker_list[i]->workplace_id]);
                        break;
                    }
                }
                break;
            }
        }
    }
    pthread_mutex_unlock(&workers_m);
}
 
void *startWorker(void *arg)
{
    //sleep(1);
    // get the arg data and save it as local
    Worker_t *worker = (Worker_t *)arg;
    char *name = worker->name;
    char *workplace = worker->workplace;
    int workplace_id = worker->workplace_id;
    int id = worker->counter;
 
    while (1)
    {
        if (getStatus(worker) == NOT_BUSY)
        {
            if (!hasFreePlace(workplace_id))
            {
                sem_wait(&sem_workplace[workplace_id]);
            }
            changePlaces(workplace_id, '-');
            setStatus(worker, BUSY);
            //printf("IhavePlace\n");
        }
 
        sem_wait(&sem_product[workplace_id]);
        //printf("IhaveProduct\n");
 
        // doing stuff, first of all - define purpose
        int data[2] = {-1, -1};
        definePurpose(data, workplace_id);
 
        // start working if I have to do smt
        if (data[PROD_SLOT] > -1 && data[STAGE_SLOT] > -1)
        {
            //printf("IhavePurp\n");
            setStatus(worker, IS_WORKING);
 
            // show info
            pthread_mutex_lock(&print_m);
            printf("%s %s %d %c\n", name, workplace, data[STAGE_SLOT] + 1, fasterProductStr(data[PROD_SLOT]));
            pthread_mutex_unlock(&print_m);
 
            // sleep (working simulation)
            usleep(howLongWait(workplace_id));
 
            // if it's the last stage of a product, then notice bout that
            if (data[STAGE_SLOT] + 1 == _PHASE_COUNT)
            {
                pthread_mutex_lock(&print_m);
                printf("done %c\n", fasterProductStr(data[PROD_SLOT]));
                pthread_mutex_unlock(&print_m);
            }
            // or else - notify next worker
            else
            {
                changeStage(data[PROD_SLOT], data[STAGE_SLOT] + 1, '+');
                int next_step_id = placeByStage(data[PROD_SLOT], data[STAGE_SLOT] + 1);
                sem_post(&sem_product[next_step_id]);
            }
 
            setStatus(worker, BUSY);
        }
 
        // check list, mb I have to remove my workplace while work
        if (removePlaceCheck(workplace_id))
        {
            changeEndPlaces(workplace_id, '-');
            setStatus(worker, NOT_BUSY);
        }
 
        // check list, mb I was called out while work
        if (checkEndList(name))
        {
            //printf("%s remove\n", name);
            removeWorker(worker);
            break;
        }
 
        pthread_mutex_lock(&global_m);
        int end = global_end;
        pthread_mutex_unlock(&global_m);
 
        if (end && noActiveWorkers(name))
        {
            for (int i = 0; i < _PLACE_COUNT; i++)
            {
                sem_post(&sem_workplace[i]);
                sem_post(&sem_product[i]);
            }
            break;
        }
    }
    // pthread_mutex_lock(&print_m);
    // printf("%s waiting till the end \n", name);
    // pthread_mutex_unlock(&print_m);
 
    sem_post(&sem_exit[id]);
 
    if (id == 0)
    {
        for (int i = 0; i < workers_count; i++)
        {
            // wait for all threads to end (get a signal)
            sem_wait(&sem_exit[i]);
        }
 
        // todo final semaphore post for all of them
        for (int i = 0; i < workers_count; i++)
        {
            sem_post(&shift_end[i]);
        }
    }
    sem_wait(&shift_end[id]);
 
    // pthread_mutex_lock(&print_m);
    // printf("%s done\n", name);
    // pthread_mutex_unlock(&print_m);
 
    return NULL;
}
 
int noActiveWorkers(char *sender_name)
{
    pthread_mutex_lock(&workers_m);
    pthread_mutex_lock(&stages_m);
 
    int res = 1;
    for (int i = 0; i < worker_list_size; i++)
    {
        if (worker_list[i])
        {
            // check all the active workers...
            if (worker_list[i]->status == IS_WORKING)
            {
                // if anyone is currently working -> leave
                // pthread_mutex_lock(&print_m);
                // printf("%s: %s is working!\n", sender_name, worker_list[i]->name);
                // pthread_mutex_unlock(&print_m);
                res = 0;
                break;
            }
            else
            {
                // if the worker is not working -> check all possible job, he could get
                if (hasJob(worker_list[i]))
                {
                    // pthread_mutex_lock(&print_m);
                    // printf("%s: %s has potential job!\n", sender_name, worker_list[i]->name);
                    // pthread_mutex_unlock(&print_m);
                    // if there is possible job for him -> leave
                    res = 0;
                    break;
                }
            }
        }
    }
 
    pthread_mutex_unlock(&stages_m);
    pthread_mutex_unlock(&workers_m);
    return res;
}
 
int hasJob(Worker_t *worker)
{
 
    int moreJob = 0;
    for (int p = 0; p < _PRODUCT_COUNT; p++)
    {
        for (int s = _PHASE_COUNT - 1; s >= 0; s--)
        {
            // find the worplace in stages
            if (placeByStage(p, s) == worker->workplace_id)
            {
                // check all stages before it, to find out - do we need to wait more or not
                moreJob = jobInStagesBefore(p, s);
                if (moreJob)
                {
                    p = _PRODUCT_COUNT;
                    break;
                }
            }
        }
    }
    return moreJob;
}
 
int jobInStagesBefore(int p, int s)
{
    int more_work = 0;
    for (int i = 0; i <= s; i++)
    {
        Worker_t *worker = findWorkerByPlace(placeByStage(p, i));
        if (stages[p][s] > 0 && worker)
        {
            // if there is any worker, who can take the job => we have to wait
            more_work = 1;
            break;
        }
    }
    return more_work;
}
 
Worker_t *findWorkerByPlace(int workplace_id)
{
    for (int w = 0; w < worker_list_size; w++)
    {
        if (worker_list[w])
        {
            if (worker_list[w]->workplace_id == workplace_id && worker_list[w]->status != NOT_BUSY)
            {
                return worker_list[w];
            }
        }
    }
    return NULL;
}
 
int removePlaceCheck(int workplace_id)
{
    pthread_mutex_lock(&end_place_m[workplace_id]);
    int ret = remove_place_list[workplace_id] > 0;
    pthread_mutex_unlock(&end_place_m[workplace_id]);
    return ret;
}
 
void definePurpose(int *data, int workplace_id)
{
    pthread_mutex_lock(&stages_m);
    // priority search
    for (int s = _PHASE_COUNT - 1; s >= 0; s--)
    {
        for (int p = 0; p < _PRODUCT_COUNT; p++)
        {
            // if stage is active & workplace is equal
            if (placeByStage(p, s) == workplace_id)
            {
                if (stages[p][s] > 0)
                {
                    data[PROD_SLOT] = p;
                    data[STAGE_SLOT] = s;
                    stages[p][s] -= 1;
                    s = -1; // break nested loops
                    break;
                }
            }
        }
    }
    pthread_mutex_unlock(&stages_m);
}
 
void removeWorker(Worker_t *worker)
{
    pthread_mutex_lock(&workers_m);
    for (int i = 0; i < worker_list_size; i++)
    {
        if (worker_list[i])
        {
            if (strcmp(worker_list[i]->name, worker->name) == 0)
            {
                free(worker->name);
                free(worker->workplace);
                free(worker);
                worker_list[i] = NULL;
                break;
            }
        }
    }
    pthread_mutex_unlock(&workers_m);
}
 
int checkEndList(char *name)
{
    pthread_mutex_lock(&workers_end_m);
    int ret = 0;
    for (int i = 0; i < worker_list_size; i++)
    {
        if (end_worker_list[i])
        {
            if (strcmp(end_worker_list[i], name) == 0)
            {
                ret = 1;
                end_worker_list[i] = NULL;
                break;
            }
        }
    }
    pthread_mutex_unlock(&workers_end_m);
    return ret;
}
 
void setStatus(Worker_t *worker, int status)
{
    pthread_mutex_lock(&workers_m);
    worker->status = status;
    pthread_mutex_unlock(&workers_m);
}
 
int getStatus(Worker_t *worker)
{
    pthread_mutex_lock(&workers_m);
    int ret = worker->status;
    pthread_mutex_unlock(&workers_m);
    return ret;
}
 
void changeStage(int product, int stage, char op)
{
    pthread_mutex_lock(&stages_m);
    if (stage < _PHASE_COUNT && stage >= 0 && product >= 0 && product < _PRODUCT_COUNT)
        stages[product][stage] += (op == '+') ? 1 : -1;
    pthread_mutex_unlock(&stages_m);
}
 
void changePlaces(int workplace_id, char op)
{
    pthread_mutex_lock(&place_m[workplace_id]);
    if (workplace_id >= 0 && workplace_id <= _PLACE_COUNT)
    {
        ready_places_list[workplace_id] += op == '+' ? 1 : -1;
    }
    pthread_mutex_unlock(&place_m[workplace_id]);
}
 
int hasFreePlace(int workplace_id)
{
    pthread_mutex_lock(&place_m[workplace_id]);
    int ret = ready_places_list[workplace_id] > 0;
    pthread_mutex_unlock(&place_m[workplace_id]);
    return ret;
}
 
void changeEndPlaces(int workplace_id, char op)
{
    pthread_mutex_lock(&end_place_m[workplace_id]);
    if (workplace_id >= 0 && workplace_id <= _PLACE_COUNT)
    {
        remove_place_list[workplace_id] += op == '+' ? 1 : -1;
    }
    pthread_mutex_unlock(&end_place_m[workplace_id]);
}
 
int find_string_in_array(const char **array, int length, char *what)
{
    for (int i = 0; i < length; i++)
        if (strcmp(array[i], what) == 0)
            return i;
    return -1;
}
 
int placeByStage(int product, int stage)
{
    int parts[_PRODUCT_COUNT][_PHASE_COUNT] = {
        {NUZKY, VRTACKA, OHYBACKA, SVARECKA, VRTACKA, LAKOVNA},
        {VRTACKA, NUZKY, FREZA, VRTACKA, LAKOVNA, SROUBOVAK},
        {FREZA, VRTACKA, SROUBOVAK, VRTACKA, FREZA, LAKOVNA}};
    return parts[product][stage];
}
 
float howLongWait(int workplace_id)
{
    float times[7] = {100, 200, 150, 300, 400, 250, 500};
    return (times[workplace_id] * 1000);
}
 
char fasterProductStr(int product)
{
    char product_c[3] = {'A', 'B', 'C'};
    return product_c[product];
}
