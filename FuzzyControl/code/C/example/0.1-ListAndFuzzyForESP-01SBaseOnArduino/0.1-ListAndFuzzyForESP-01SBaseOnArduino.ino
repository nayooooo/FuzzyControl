#include <Arduino.h>

#include "list.h"
#include "fuzzy_opera.h"

#ifndef UNUSED
#   define UNUSED(data) ((void)data)
#endif

static bool int_trav(list_node n, void* data)
{
    UNUSED(data);

    int *num = (int*)(n->data);
    Serial.printf("%d  ", *num);

    return true;
}

static bool int_prev_eq(list_node n, void* data)
{
    return *(int*)(n->data) == *(int*)data;
}

static bool int_prev_even(list_node n, void* data)
{
    UNUSED(data);

    return ((*(int*)(n->data) > 0) && (*(int*)(n->data) % 2 == 0));
}

extern "C" int arduino_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int len = vsnprintf(NULL, 0, format, args) + 1;
    char* buffer = (char*)malloc(len * sizeof(char));
    if (buffer == NULL) {
        return -1;
    }
    vsnprintf(buffer, len, format, args);
    va_end(args);

    Serial.printf("%s", buffer);

    free(buffer);
    return len;
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.write("\r\n");

    // test list
    {
        int a = 3;
        int b = 11;
        list_head l;
        
        Serial.write("\r\n\n");
        l = list_create();
        if (l == nullptr)
        {
            Serial.write("list create failed!\r\n");
            goto list_over;
        }
        Serial.printf("[create] list address: 0x%X\r\n", l);
        Serial.printf("[create] list node number: %u\r\n", list_length(l));

        Serial.write("\r\n\n");
        for (int i = 0; i < 20; i++)
        {
            list_push(l, &i, sizeof(i));
        }
        Serial.printf("[push] list node number: %u\r\n", list_length(l));
        Serial.write("[push] node data: \r\n");
        list_trav(l, nullptr, int_trav);
        Serial.write("\r\n");

        Serial.write("\r\n\n");
        for (int i = 0; i < 20; i++)
        {
            list_remove_if(l, nullptr, int_prev_even, nullptr);
        }
        Serial.printf("[remove_if] list node number: %u\r\n", list_length(l));
        Serial.write("[remove_if] node data: \r\n");
        list_trav(l, nullptr, int_trav);
        Serial.write("\r\n");

        Serial.write("\r\n\n");
        for (int i = 0; i < 3; i++)
        {
            list_pop(l);
        }
        Serial.printf("[pop] list node number: %u\r\n", list_length(l));
        Serial.write("[pop] node data: \r\n");
        list_trav(l, nullptr, int_trav);
        Serial.write("\r\n");

        Serial.write("\r\n\n");
        list_swap_if(l, &a, &b, int_prev_eq, int_prev_eq);
        Serial.printf("[swap_if] list node number: %u\r\n", list_length(l));
        Serial.write("[swap_if] node data: \r\n");
        list_trav(l, nullptr, int_trav);
        Serial.write("\r\n");
        
    list_over:
        Serial.write("\r\n\n");
        list_delete(l, nullptr);
        Serial.printf("[delete] list node number: %u\r\n", list_length(l));
    }

    // test fuzzy matrix
    {
        struct fuzzy_matrix mat;
        struct fuzzy_matrix mat2;
        struct fuzzy_matrix mat3;
        fuzzy_matrix_init(&mat);
        fuzzy_matrix_init(&mat2);
        fuzzy_matrix_init(&mat3);

        Serial.write("\r\n\n");
        fuzzy_matrix_create(&mat, 3, 4);
        fuzzy_matrix_set_elem(&mat, 0.1145);
        fuzzy_matrix_print(&mat, "[mat] ori");

        Serial.write("\r\n\n");
        fuzzy_matrix_reshape(&mat, 2, 2);
        fuzzy_matrix_print(&mat, "[mat] reshape to (2,2)");
        fuzzy_matrix_reshape_s(&mat, 5, 5);
        fuzzy_matrix_print(&mat, "[mat] reshape_s to (5,5)");

        
        Serial.write("\r\n\n");
        fuzzy_matrix_copy(&mat2, &mat);
        fuzzy_matrix_print(&mat2, "[mat2] copy mat");
        
        Serial.write("\r\n\n");
        fuzzy_matrix_delete(&mat);
        fuzzy_matrix_delete(&mat2);
        fuzzy_matrix_create(&mat, 2, 3);
        for (fuzzy_size r = 0; r < mat.row; r++)
        {
            for (fuzzy_size c = 0; c < mat.col; c++)
            {
                mat.mat[r][c] = r * mat.col + c + 1;
            }
        }
        fuzzy_matrix_print(&mat, "[mat] ori");
        fuzzy_matrix_create(&mat2, 2, 2);
        fuzzy_matrix_set_elem(&mat2, 1);
        fuzzy_matrix_print(&mat2, "[mat2] ori");
        fuzzy_matrix_horzcat(&mat3, &mat, &mat2);
        fuzzy_matrix_print(&mat3, "[mat3] horzcat mat and mat2");
        
        Serial.write("\r\n\n");
        fuzzy_matrix_print(&mat, "[mat] ori");
        fuzzy_opera_transpose(&mat, nullptr);
        fuzzy_matrix_print(&mat, "[mat] trans");
        
        Serial.write("\r\n\n");
        fuzzy_matrix_delete(&mat);
        fuzzy_matrix_delete(&mat2);
        fuzzy_matrix_create(&mat, 2, 2);
        for (fuzzy_size r = 0; r < mat.row; r++)
        {
            for (fuzzy_size c = 0; c < mat.col; c++)
            {
                mat.mat[r][c] = r * mat.col + c + 1;
            }
        }
        fuzzy_matrix_create(&mat2, 2, 4);
        for (fuzzy_size r = 0; r < mat2.row; r++)
        {
            for (fuzzy_size c = 0; c < mat2.col; c++)
            {
                mat2.mat[r][c] = r * mat2.col + c + 1;
            }
        }
        fuzzy_matrix_print(&mat, "[mat] ori");
        fuzzy_matrix_print(&mat2, "[mat2] ori");
        bool ret = fuzzy_opera_dir_pro(&mat3, &mat, &mat2);
        if (ret == false)
        {
            Serial.write("dir_pro failed!\r\n");
        }
        else
        {
            fuzzy_matrix_print(&mat3, "[mat3] dir_pro");
        }
        fuzzy_matrix_delete(&mat2);
        fuzzy_matrix_create(&mat2, 1, 4);
        for (fuzzy_size r = 0; r < mat2.row; r++)
        {
            for (fuzzy_size c = 0; c < mat2.col; c++)
            {
                mat2.mat[r][c] = r * mat2.col + c + 1;
            }
        }
        fuzzy_matrix_print(&mat, "[mat] ori");
        fuzzy_matrix_print(&mat2, "[mat2] ori");
        fuzzy_opera_dir_pro(&mat3, &mat, &mat2);
        fuzzy_matrix_print(&mat3, "[mat3] dir_pro");

    fuzzy_matrix_over:
        Serial.write("\r\n\n");
        fuzzy_matrix_delete(&mat);
        fuzzy_matrix_delete(&mat2);
        fuzzy_matrix_delete(&mat3);
    }

    Serial.write("Done!\r\n");
}

void loop() {
  // put your main code here, to run repeatedly:
    delay(1);
}
