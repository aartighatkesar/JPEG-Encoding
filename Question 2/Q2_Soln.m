%% Lab 9: Achromatic Baseline JPEG Encoding Lab
% Author; Aarti Ghatkesar
% Exercise 2.3: Observe Properties of DC and AC coefficients

%%

clc
clear all
close all

%% Reading DCT coefficients from 'img03y_1.dq' file obtained from previous section

run('jpeg\Qtables.m');
fileID = fopen('img03y_1.dq','r');
A= fread(fileID,'integer*2');
fclose(fileID);
rows = A(1);
cols = A(2);
quant_dct_coeff = reshape(A(3:end),cols,rows);
quant_dct_coeff = quant_dct_coeff';


%% Displaying DC coefficients
fcn = @(x)(x.data(1));
dct_blk = blockproc(quant_dct_coeff,[8,8],fcn);
dct_blk = dct_blk + 128;
imshow(dct_blk,[]);

%% Extracting AC coefficients

AC_coeff = zeros(rows*cols/64,63);
count =1;

for  i = 1:1:rows/8
    for j = 1:1:cols/8
      
        blk = quant_dct_coeff((i-1)*8+1:i*8,(j-1)*8+1:j*8);
        blk = blk(Zig);
        blk=blk(2:end);
        AC_coeff(count,:) = blk;
        count = count + 1;
        
        
    end
end

avg = mean(abs(AC_coeff));
figure
plot([2:64]',avg);
xlabel('AC coefficient index in Zig Zag order');
ylabel('Mean absolute value of AC coefficients across all blocks');
title('Mean absolute value of AC coefficients in zig zag order');


