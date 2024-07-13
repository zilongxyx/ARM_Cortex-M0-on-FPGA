x=60
y=80
img888=imread('F:\jichuangsai\week1_test\img\3.png');
 
scaleR=31/255;  
scaleG=63/255;
scaleB=31/255;
 
img565=zeros(x,y);
 
for i=1:x
    for j=1:y
        r5=0;
        g6=0;
        b5=0;
        r5=round(double(img888(i,j,1))*scaleR);
        g6=round(double(img888(i,j,2))*scaleG);
        b5=round(double(img888(i,j,3))*scaleB);
        r5L11=bitshift(r5,11);  %r左移11位
        g6L5=bitshift(g6,5);    %g左移5位
        img565(i,j)=r5L11+g6L5+b5;
    end
end
img565=dec2hex(uint16(img565));

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