# B+ Tree 實作

## 程式操作
1. 進入 code 資料夾
2. `$ make` : 會編譯並產生執行檔，執行檔檔名為 B+Tree
3. `$ ./B+Tree` : 執行程式
4. 依照提示字輸入相關資訊
最一開始會需要先輸入 order 大小，之後可以使用以下指令
    - `> insert data1 data2 ...` : 插入 data，可以有一至多個 data，會依序插入
    - `> insert -r data1 data2 ...` : 插入 data，可以有一至多個 data，會依序插入，並使用 redistribute
    - `> insert -b data1 data2 ...` : 插入 data，可以有一至多個 data，會使用 bulk loading (只有樹是空的時候才能使用這個指令)
    - `> delete data1 data2 ...` : 刪除 data，可以有一至多個 data，會依序刪除
    - `> search data` : 搜尋 data，搜尋單一值存不存在
    - `> search operator data` : 範圍搜尋，需搭配 operator (<=, <, >=, >) 使用，會把符合條件的值都印出
    - `> display` : 印出 B+ tree
    - `> quit` : 結束程式 
    - `> help` : 印出使用說明


[🔗 Demo 影片](https://www.youtube.com/watch?v=um98XIR9P1o)           

