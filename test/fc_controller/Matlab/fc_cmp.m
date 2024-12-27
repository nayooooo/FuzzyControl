%%
clear;clc;
%%
file = fopen(['data/1E-1f/time.bin'], 'r');
row1 = fread(file, 1, 'int32');
col1 = fread(file, 1, 'int32');
result1 = fread(file, [row1, col1], 'float');
fclose(file);

file = fopen(['data/1E-2f/time.bin'], 'r');
row2 = fread(file, 1, 'int32');
col2 = fread(file, 1, 'int32');
result2 = fread(file, [row2, col2], 'float');
fclose(file);

file = fopen(['data/1E-3f/time.bin'], 'r');
row3 = fread(file, 1, 'int32');
col3 = fread(file, 1, 'int32');
result3 = fread(file, [row3, col3], 'float');
fclose(file);
%%
delta13 = result1 - result3;
error13 = delta13 ./ result3;
[r, c] = size(error13);
err13 = sum(sum(error13)) / (r * c);
disp(['1/3: ' num2str(err13 * 100) '%']);

delta23 = result2 - result3;
error23 = delta23 ./ result3;
[r, c] = size(error23);
err23 = sum(sum(error23)) / (r * c);
disp(['2/3: ' num2str(err23 * 100) '%']);
