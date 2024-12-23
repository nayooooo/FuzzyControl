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
err = sum(sum(error)) / (r * c);
disp(['error: ' num2str(err * 100) '%']);
