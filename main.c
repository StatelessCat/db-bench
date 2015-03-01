#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>

/* name of the sqlite database */
#define SQLITEDBNAME "/home/raphael/.local/share/baloo/file/fileMap.sqlite3"

struct idArray {
    unsigned int * id;
    unsigned int counter;
};

static int callback (void *user_data, int tuplesCount, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < tuplesCount; ++i)
    {
        //printf("%s = %s\n", azColName[i], argv[i]);
    }
    //printf ("\n");
    return 0;
}

static int fillIdArray (void *idsArr, int colCount, char **argv, char **azColName)
{
    //printf("id = %s\n", argv[0]);
    int pos = ((struct idArray *)idsArr)->counter;
    unsigned int * id = ((struct idArray *)idsArr)->id;

    id[pos] = atoi(argv[0]);

    (((struct idArray *)idsArr)->counter)++;
    return 0;
}

static int count (void *user_data, int tuplesCount, char **argv, char **azColName)
{
    unsigned int c = atoi(argv[0]);
    *((unsigned int *)user_data) = c; // unsafe
    return 0;
}

int main (int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Syntax : %s <itNumber>\n", argv[0]);
        return 1;
    }
    int max = atoi(argv[1]);

    sqlite3 * dco;              /* SQLITE database connection object */
    int rc;                     /* return code */
    unsigned int * tuplesCount;
    int r;

    tuplesCount = (unsigned int *) malloc(sizeof(unsigned int));

    srand(time(NULL));

    /* connect to the database */
    rc = sqlite3_open(SQLITEDBNAME, &dco);
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(dco));
        exit(EXIT_FAILURE);
    } else
    {
        fprintf(stdout, "DB %s [OK]\n", SQLITEDBNAME);
    }

    sqlite3_exec(dco,
            "SELECT count(*) FROM files;",
            count, tuplesCount, NULL);

    fprintf(stdout, "DB contains %d tuples \n", *tuplesCount);

    /* declare the array of ids */
    struct idArray idsArr;
    idsArr.id = malloc(sizeof(unsigned int) * *tuplesCount);
    idsArr.counter = 0;

    /* fill the array of ids*/
    sqlite3_exec(dco,
            "SELECT id FROM files;",
            fillIdArray, &idsArr, NULL);

    /* bench */
    for (int i = 1; i < max; ++i )
    {
        r = rand()%(*tuplesCount);
        unsigned int id = idsArr.id[r];

        char baseReq[34] = "SELECT * FROM files WHERE id = %d";

        // i uses 10 because max value of unsigned int needs 10 numbers to be show in string
        char * req = (char *) malloc((sizeof(char) * 33) + (sizeof(char)*10)); /* TODO fixme */
        sprintf(req, baseReq, id);

        /* request a pseudo-random tuple from the database */
        sqlite3_exec(dco, req, callback, tuplesCount, NULL);
        /* TODO don't work every time because some ID have been deleted */

        free(req);
    }

    free(idsArr.id);
    free(tuplesCount);
    exit(EXIT_SUCCESS);
}
