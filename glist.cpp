/*
 * compilation
 * g++ glist.cpp `pkg-config --cflags --libs glib-2.0`
 * */

#include <iostream>
#include <stdlib.h>
#include <glib.h>

using namespace std;

typedef struct _foo
{
    int a;
    int b;
}foo;

void print_number (gpointer f, gpointer ignored)
{
    cout << "*****************  data = " << ((foo *)f)->b << endl;
}

void print_list (GList *list)
{
    if (list == NULL)
    {
        g_print ("Empty List\n");
    }
    else
    {
        g_list_foreach (list, print_number, NULL);
        g_print ("\n");
    }
}

int main (int argc, char *argv[])
{
    GList *list = NULL;
    foo *f = NULL;

    for (gint i = 0; i < 10; i++)
    {
        f = g_new (foo, 1);
        f->b = i+1;
        list = g_list_append (list, (gpointer) f);
    }

    /*Add a few more elements into list*/
    for (gint j = 0; j < 3; j++)
    {
        f = g_new (foo, 1);
        f->b = j+1;
        list = g_list_append (list, (gpointer) f);
    }

    /*Add an element 100 after 3rd element*/
    f = g_new (foo, 1);
    f->b = 100;
    list = g_list_insert (list, (gpointer) f, 3);

    /*Find element previsously inserted and insert 200 before it*/
    GList *tmp;
    tmp = g_list_find (list, f);
    f = g_new (foo, 1);
    f->b = 200;
    list = g_list_insert_before (list, tmp, (gpointer) f);

    /*Find out the element at 10th position (starting from 0) and print it*/
    foo *t;
    t = (foo *) g_list_nth_data (list, 10);
    cout << "t->b = " << t->b << endl;
    /*Find out the element at 6th position (starting from 0) and print it*/
    tmp = g_list_nth (list, 6);
    t = (foo*) tmp->data;
    cout << "t->b = " << t->b << endl;

    /*Remove 7th element from the list*/
    list = g_list_remove (list, g_list_nth_data (list, 7));

    /*Add 7th element to the list*/
    f = g_new (foo ,1);
    f->b = 6;
    list = g_list_insert_before (list, g_list_nth (list, 7), f);

    print_list (list);
    
}
























