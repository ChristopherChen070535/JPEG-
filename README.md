### 用c實現JPEG
在code資料夾中有我完成的程式碼(matlab code是我寫這個程式主要參考的程式碼，主要是從matlab改寫為c)以及程式需要的圖檔，目前我寫到使用方法1(b)，程式碼是decoder_adderror.c(我還沒併入decoder.c中)，遇到的問題是在進行decode時輸出的圖片是錯誤的(目前發現是將error加回去時發生了問題)，如下圖所示，希望各位大神可以幫我看看是否是哪一個環節出了問題，因為本人第一次接觸這種較大型的專案，code寫法較不成熟，造成各位麻煩了   
這裡是關於這次專案的題目解釋:
https://www.youtube.com/playlist?list=PL6i9vEYOx2Bu1L20sHtb3VREQKG4Q9tJf  
這裡是關於這次專案的原理影片以及講義:
https://www.youtube.com/playlist?list=PL6i9vEYOx2Bv5AabzBLteR8PDmuT5TUQZ  
https://github.com/cychiang-ntpu/ntpu-ce-mmsp-2023/tree/master/Chapter-5

![image](https://github.com/Jeremy411086013/JPEG-/blob/main/output.bmp)https://github.com/Jeremy411086013/JPEG-/blob/main/output.bmp)
