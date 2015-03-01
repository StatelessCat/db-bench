#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>

/* name of the sqlite database */
#define SQLITEDBNAME "/home/raphael/.local/share/baloo/file/fileMap.sqlite3"

static int callback (void *user_data, int tuplesCount, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < tuplesCount; ++i)
    {
        printf("%s = %s\n", azColName[i], argv[i]);
    }
    printf ("\n");
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

    r = rand()%(*tuplesCount);
    char baseReq[34] = "SELECT * FROM files WHERE id = %d";

    // i uses 10 because max value of unsigned int needs 10 numbers to be show in string
    char * req = (char *) malloc((sizeof(char) * 33) + (sizeof(char)*10)); /* TODO fixme */
    sprintf(req, baseReq, r);

    /* request a pseudo-random tuple from the database */
    sqlite3_exec(dco, req, callback, tuplesCount, NULL);
    /* TODO don't work every time because some ID have been deleted */

    free(tuplesCount);
    free(req);

    exit(EXIT_SUCCESS);
}
