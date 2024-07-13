%生成C语言数组
str="{0x"
a=dec2hex(img565(1))
for i=1:x*y
    for j=1:4
        str=strcat(str,img565(i,j));            
    end
    str=strcat(str,",0x");
end
str=strcat(str,'}');
writematrix(str,'F:\jichuangsai\week1_test\img\1.txt')