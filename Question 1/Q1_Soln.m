%% Lab 9: Achromatic Baseline JPEG Encoding Lab
% Author; Aarti Ghatkesar
% Exercise 2.1: Block DCT transforms followed by Quantization - Determine
% effect of gamma


%%

clc
clear
close all

%% Reading Image and converting to double and level shifting by 128

img = double(imread('img03y.tif'));
img = img - 128;

%% Executing script 'Qtables.m' for variables Quant and Zig
run('jpeg\Qtables.m');

%% 8 x 8 Block DCT and Quantization

gm = 4;

fcn = @(x)round(dct2(x.data,[8,8])./(Quant*gm));
dct_blk = blockproc(img,[8,8],fcn);


%% Writing to file

fileID =fopen('img03y_4.dq','w');
fwrite(fileID,size(img),'integer*2');
fwrite(fileID,dct_blk','integer*2');
fclose(fileID);

%% Reading from Binary file

fileID = fopen('img03y_4.dq','r');
A= fread(fileID,'integer*2');
fclose(fileID);
rows = A(1);
cols = A(2);
quant_dct_coeff = reshape(A(3:end),cols,rows);
quant_dct_coeff = quant_dct_coeff';

%% Inverse operations

fcn = @(x)(x.data.*Quant*gm);
dct_blk = blockproc(quant_dct_coeff,[8,8],fcn);

fcn = @(x)round((idct2(x.data,[8,8])));
restored_img = blockproc(dct_blk,[8,8],fcn);
figure
imshow(restored_img,[]);
title(sprintf('Restored Image gamma = %f',gm),'Interpreter','LaTex');

diff_img = (img - restored_img).*10 +128;
figure
imshow(diff_img,[]);
% title('Difference Image');


