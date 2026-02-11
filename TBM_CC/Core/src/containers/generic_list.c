// /**
//  * @authors   Ario Amin @ Permadev, 
//  * @copyright Copyright (c) 2021-2026, MIT-License included in project toplevel dir
//  */

// #include "containers/generic_list.h"
// #include "sys/heap.h"


// BaseList* list_new_(int size_of_each_element, int element_count)
// {
//     BaseList* new_list = malloc_(sizeof(BaseList));
//     new_list->single_element_size = size_of_each_element;
//     list_new_buffer_(new_list, element_count);
//     return new_list;
// }

// void list_new_buffer_(void* existing_list, int element_count)
// {
//     BaseList* _existing_list = (BaseList*)existing_list;
//     _existing_list->buffer = malloc_(_existing_list->single_element_size * element_count);
//     _existing_list->index = 0;
//     _existing_list->length = 0;
//     _existing_list->capacity = element_count;
// }

// void list_resize(void* list, int new_size)
// {
//     BaseList* _list = (BaseList*)list;

//     // _list->buffer = realloc_(_list->buffer, new_size); @todo Implement realloc_
//     _list->capacity = new_size;
// }

// void list_delete_buffer(void* list)
// {
//     BaseList* _list = (BaseList*)list;

//     if (_list->capacity == 0) { return; }
    
//     free_(_list->buffer);
//     _list->index = _list->length = _list->capacity = 0;
// }

// void list_delete(void* list)
// {
//     BaseList* _list = (BaseList*)list;
//     list_delete_buffer(_list);
//     free_(_list);
// }

// void list_push(void *list, , ... /* void* data */)
// {
//     va_list args;
//     va_start(args, b);

//     while(arg = va_arg(ap, int))
//     {
//         Sum += arg;
//     }
//     va_end(ap);

//     return Sum;    

//     BaseList* _list = (BaseList*)list;
//     uint8_t* _element = (uint8_t*)data;
//     if (_list->length >= _list->capacity) { list_resize(_list, _list->capacity * 2); }

//     uint8_t* buffer = (uint8_t*)_list->buffer;

//     int32_t selected_byte = _list->single_element_size * _list->index;
//     buffer[ selected_byte ] = *_element;

//     _list->index++;
//     _list->length++;
// }