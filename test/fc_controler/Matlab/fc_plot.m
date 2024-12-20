%%
clear;clc;
USE_DATA_PATH = 'data/1E-1f';
USE_DATA_RESULT = 'time.bin';
%%
file = fopen([USE_DATA_PATH '/' USE_DATA_RESULT], 'r');
row = fread(file, 1, 'int32');
col = fread(file, 1, 'int32');
result = fread(file, [row, col], 'float');
fclose(file);

x = 0:100;
y = 0:100;
[X, Y] = meshgrid(x, y);
%%
figure();
mesh(X, Y, result);
xlabel('污泥(sludge)');
ylabel('油脂(grease)');
zlabel('时间(time)');
title('模糊输出');
grid on;
