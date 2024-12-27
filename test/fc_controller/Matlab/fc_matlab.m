%%
% clear;clc;
%%
load('FuzzyMatlab.mat');
resualt_mat = zeros(101);
for i = 0:1:100
    for j = 0:1:100
        resualt_mat(i + 1, j + 1) = evalfis(FuzzyMatlab, [i j]);
    end
end
%%
delta = result - resualt_mat;
error = delta ./ resualt_mat;
[r, c] = size(error);
err = sum(sum(abs(error))) / (r * c);
disp(['error: ' num2str(err * 100) '%']);
%%
x = 0:100;
y = 0:100;
[X, Y] = meshgrid(x, y);
figure();
mesh(X, Y, error);
xlabel('污泥(sludge)');
ylabel('油脂(grease)');
zlabel('相对误差');
title('模糊输出的相对误差');
