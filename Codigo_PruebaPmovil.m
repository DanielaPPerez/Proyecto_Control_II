clear; clc; close all;
Pot=importdata('Potenciometro_0a4_Kp68.xlsx'); Ref=importdata('Referencia_0a4_Kp68.xlsx'); %Importa los datos
%%
Ts=Pot(2,1)/(1e6); Offset=0.13; % Periodo de musetreo y Offset de la señal
Volt_Pot=Pot(:,2)-Offset; Volt_Ref=Ref(:,2)-Offset; %Extracción de datos removiendo el offset
%%
m=40; Volt_Pot=pmovil(Volt_Pot,m); Volt_Ref=pmovil(Volt_Ref,m); %Promedio Movil
%%
plot(Volt_Pot(190:end-200)); hold on; plot(Volt_Ref(190:end-200)); %Gráfica de la señal
%%
s=tf('s'); z=tf('z',Ts); %Definir variables como tf
Pcr=0.688; Kcr=68; %Periodo y ganancia críticos
Pcr2=0.162;

% kp=0.5*Kcr; %Constante del P
% kp=0.45*Kcr; ki=kp/(Pcr2/1.2); kd=0; %Constantes del PI
kp=0.6*Kcr; ki=kp/(Pcr2/2); kd=kp*0.125*Pcr2; %Constantes del PID

Gc=kp+ki/(s)+kd*s;
% dGc=c2d(Gc,Ts)
dGc=kp+kd*(z-1)/(Ts*z)

%%
function [y] = pmovil (x, M)
%Edit here %%%%%%%%%%%%%%%%%%%%
u(1:2*M+1)=1;
hn=1/(2*M + 1)*u;
y=conv(x,hn);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end
%%
%Función de transferencia en lazo cerrado
% syms s HLA
% Gcl=0.1857/(s^2+0.7452*s+0.235);
% k=1;
% ec=Gcl==k*HLA/(1+k*HLA);
% Glas=solve(ec,HLA);%Hallo función de transferencia lazo abierto

%Función de transferencia en lazo abierto
% s=tf('s');
% Gla=1857/(10000*s^2 + 7452*s + 493);
% dGla=c2d(Gla,Ts);

% figure; rlocus(dGla)
% figure; rlocus(Gla)
