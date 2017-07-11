#pragma once
namespace practicellvm{
    //安全な削除
    template<class T>
    void Safe_Delete(T&& t){delete t;t=nullptr;}
    //安全な削除（配列版）
    template<class T>
    void Safe_Delete_Array(T&& t){delete[] t;t=nullptr;}
}