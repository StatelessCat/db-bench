#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

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
    exit(EXIT_SUCCESS);
}

static int count (void *user_data, int tuplesCount, char **argv, char **azColName)
{
    unsigned int c = atoi(argv[0]);
    *((unsigned int *)user_data) = c; // unsafe
    return 0;
}

int main (int argc, char **argv)
{
    sqlite3 * dco; /* SQLITE database connection object */
    int rc;        /* return code */
    unsigned int * tuplesCount = (unsigned int *) malloc(sizeof(unsigned int));

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

    exit(EXIT_SUCCESS);
}
