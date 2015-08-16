#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUERY_LEN 256


typedef enum { FALSE_B, TRUE_B } bool_t;
bool_t first_json_row;

// Functions

int callback_eachRow(void *pointer, int argc, 
        char **argv, char **column_name);

int shell_sqlite(sqlite3 *db);

void shell_startup();


int main(int argc, char *argv[])
{
    sqlite3 *db;
    first_json_row = TRUE_B;

    // db parameter
    char *db_name;
    if ( argc == 2 )
         db_name = argv[1];
    else 
        goto error;

    // Open db
    int retval;
    retval = sqlite3_open(db_name, &db);
    if (retval != SQLITE_OK) {
        sqlite3_close(db);
        goto error;
    }

    shell_startup();

    shell_sqlite(db);

    sqlite3_close(db);

    return 0;
error:
    printf("error\n");
    return -1;
}


/* Called for each row returned by query */
int callback_eachRow(void *pointer, int argc, 
        char **argv, char **column_name) 
{
    pointer = 0;
    
    // json print stuff
    if ( first_json_row == TRUE_B) {
        printf("\n[{");
        first_json_row = FALSE_B;
    }
    else 
        printf(",\n {");

    int i;
    for (i = 0; i < argc; i++) {
        printf("\"%s\":", column_name[i]);
        int rc = atoi(argv[i]);
        // Is Number
        if (rc != 0 ) {
            if ( i < argc-1) printf("%s, ",argv[i]);
            else printf("%s", argv[i] );
        }
        // Is String
        else {
            if ( i < argc-1) printf("\"%s\", ",argv[i]);
            else printf("\"%s\"", argv[i] );
        }
    }
    printf("}");

    return 0;
}

void sql_execute(sqlite3 *db, char *sql)
{

    // Execute SQL query
    int retval;
    char *err_msg = 0;
    retval = sqlite3_exec(db, sql, callback_eachRow, 0, &err_msg);

    if (retval != SQLITE_OK ) {
        fprintf(stderr, "error: %s\n", err_msg);
        sqlite3_free(err_msg);        
    }
    else if ( first_json_row == FALSE_B )
        printf("]\n");

    first_json_row = TRUE_B;
}

int shell_sqlite(sqlite3 *db)
{

    int retval;
    char sql_string[MAX_QUERY_LEN];
    while (1) {
        printf("sqlite-json> ");
        fgets (sql_string, MAX_QUERY_LEN, stdin);

        char *sql;
        if ( (strcmp( sql_string, ".tables\n") ) == 0 ) {
            sql= "SELECT name FROM sqlite_master WHERE type='table'\n";
        }
        else if ( (strcmp( sql_string, ".quit\n") ) == 0 )
            break;
        else
            sql = sql_string;

        sql_execute(db, sql);
    }

    return 0;
}

void shell_startup()
{
    printf(" \
=========== \n \
sqlite-json \n \
=========== \n \
        Enter SQL statements  \n \
        Enter \".quit\" to exit \n \
        Enter \".tables\" for tables \n \
    \n");
}

