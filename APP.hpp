#pragma once

template<class T>
void Safe_Delete(T&& t){delete t;t=nullptr;}
template<class T>
void Safe_Delete_Array(T&& t){delete[] t;t=nullptr;}
