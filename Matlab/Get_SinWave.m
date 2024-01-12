% 可更改的参数
num_points      =   65535;   % 点数
amplitude_mv    =   3.0;        % 振幅（伏）

% 生成正弦波数组
theta = linspace(0, 2*pi, num_points + 1);
U = amplitude_mv / 2;
scaled_sinusoidal_wave = round((U * sin(theta) + U) * 4095 / 3.3) + 300;

% 宏定义表示点数
fprintf('#ifndef DDS_ROM_SIN_WAVE_SIZE\n');
fprintf('#define DDS_ROM_SIN_WAVE_SIZE    (%d)\n', num_points);
fprintf('#endif\n\n');

% 打印数组数据
fid = fopen('../Platform/Platform-lib/DSP/Waveform_Table/SinWave.h', 'w');
fprintf(fid, '#ifndef DDS_ROM_SIN_WAVE_SIZE\n');
fprintf(fid, '#define DDS_ROM_SIN_WAVE_SIZE    (%d)\n', num_points);
fprintf(fid, '#endif\n\n');
fprintf(fid, 'const uint16_t SinWave_ROM[DDS_ROM_SIN_WAVE_SIZE] = {\n');
fprintf(fid, '    %d', scaled_sinusoidal_wave(1));
for i = 2:num_points
    fprintf(fid, ', %d', scaled_sinusoidal_wave(i));
    if mod(i, 10) == 0  % 每十个数换一行，可根据需要调整
        fprintf(fid, '\n');
    end
end
fprintf(fid, '};\n');
fclose(fid);

% 绘制图形
figure;
plot(scaled_sinusoidal_wave);
xlabel('数组下标');
ylabel('DAC量化值');
title('SinWave ROM');
