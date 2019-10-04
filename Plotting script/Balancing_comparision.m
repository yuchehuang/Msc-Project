clear
%% Loading txt file dir default Desktop
MakKov='Markov Balancing StateLog.txt';
Nom_MakKov='Non Markov Balancing StateLog.txt';
MakKov_data=dlmread(MakKov);
MakKov_Size= size(MakKov_data);

Nom_MakKov_data=dlmread(Nom_MakKov);
Nom_MakKov_Size= size(Nom_MakKov_data);

t=(0.01:0.01:60);
col=1;

%% -3.14  Non Markov
% for i=1:Nom_MakKov_Size
%     if Nom_MakKov_data(i,col)<0
%         Nom_MakKov_data(i,col)=Nom_MakKov_data(i,col)+3.14;
%     else
%         Nom_MakKov_data(i,col)=Nom_MakKov_data(i,col)-3.14;
%     end
% end

%% -3.14  Markov
% for i=1:MakKov_Size
%     if MakKov_data(i,col)<0
%         MakKov_data(i,col)=MakKov_data(i,col)+3.14;
%     else
%         MakKov_data(i,col)=MakKov_data(i,col)-3.14;
%     end
% end
%% Low pass Filter (MakKov)
Temp=MakKov_data(:,col)';
LPF_parameter=[0.5 0.5];
LPF= conv2(LPF_parameter,Temp);
% Resize the matrix due to the LPF will increase the length of the martix 
LPF =LPF(1,1:MakKov_Size);


%% Low pass Filter (Non MakKov)
Temp_Non=Nom_MakKov_data(:,col)';
LPF_parameter=[0.5 0.5];
Non_LPF= conv2(LPF_parameter,Temp_Non);
% Resize the matrix due to the LPF will increase the length of the martix 
Non_LPF =Non_LPF(1,1:Nom_MakKov_Size);

%% MAF 
%  Low_data=data(:,col)';
%  for Loop=1:5
%     M=2;
%     for length=1:(Size(1)-(M-1))
%         temp=0;
%         for element=length:(length+(M-1))
%             temp =Low_data(1,element)+temp;
%         end 
%         Low_data(1,length)=(temp)/M;
%     end 
%      
%  end

%% Responding time info
SettlingTimeThreshold=0.01;
disp('MakKov_Sys:')
Markov_result=stepinfo(MakKov_data(:,col),t,'SettlingTimeThreshold',SettlingTimeThreshold)
Markov_x=Markov_result.SettlingTime;
Markov_y=MakKov_data(fix(100*Markov_x),col);

Markov_RiseTime=sprintf('RiseTime: %f', Markov_result.RiseTime)
Markov_SettlingTime=sprintf('SettlingTime: %f', Markov_result.SettlingTime)
Markov_SettlingMin=sprintf('SettlingMin: %f', Markov_result.SettlingMin)
Markov_SettlingMax=sprintf('SettlingMax: %f', Markov_result.SettlingMax)
Markov_Overshoot=sprintf('Overshoo:t %e', Markov_result.Overshoot)
Markov_Undershoot=sprintf('Undershoo:t %e', Markov_result.Undershoot)
Markov_Peak=sprintf('Peak: %f', Markov_result.Peak)

disp('=========================')
disp('Non MakKov_Sys:')
Non_Markov_result=stepinfo(Nom_MakKov_data(:,col),t,'SettlingTimeThreshold',SettlingTimeThreshold)
Non_Markov_x=Non_Markov_result.SettlingTime;
Non_Markov_y=Nom_MakKov_data(fix(100*Non_Markov_x),col);

Non_Markov_RiseTime=sprintf('RiseTime: %f', Non_Markov_result.RiseTime)
Non_Markov_SettlingTime=sprintf('SettlingTime: %f', Non_Markov_result.SettlingTime)
Non_Markov_SettlingMin=sprintf('SettlingMin: %f', Non_Markov_result.SettlingMin)
Non_Markov_SettlingMax=sprintf('SettlingMax: %f', Non_Markov_result.SettlingMax)
Non_Markov_Overshoot=sprintf('Overshoo:t %e', Non_Markov_result.Overshoot)
Non_Markov_Undershoot=sprintf('Undershoo:t %e', Non_Markov_result.Undershoot)
Non_Markov_Peak=sprintf('Peak: %f', Non_Markov_result.Peak)

%% Plot all data

figure(1);

plot(t,MakKov_data(:,col),'LineWidth',1)
hold on
line([Markov_x Markov_x], [Markov_y -0.05],'Color','Blue','LineStyle','--');
plot(t,Nom_MakKov_data(:,col),'r','LineWidth',1)
line([Non_Markov_x Non_Markov_x], [Non_Markov_y -0.05],'Color','red','LineStyle','--');

%plot(LPF(1,:),'LineWidth',1)
%plot(abs(Nom_MakKov_data(:,col)),'r','LineWidth',1)
%plot(Non_LPF(1,:),'r','LineWidth',1)
hold off
ylabel('Theta 2 (rad)')
xlabel('Time (sec)');
Title=sprintf('Balancing  task,  Settling Threshold: %f %%',100*SettlingTimeThreshold)
title(Title);
legend('MarKov','MarKov SettlingTime','Non-MarKov','NOn MarKov SettlingTime')

%% Non Markov textbox
x=.4;
Non_dim = [x+0.15 .6 .3 .3];
%Non_dim = [.75 .1 .3 .3];
Non_str = {'Non Markov Solution:',Non_Markov_RiseTime,Non_Markov_SettlingTime,Non_Markov_SettlingMin,Non_Markov_SettlingMax,Non_Markov_Overshoot,Non_Markov_Undershoot,Non_Markov_Peak}
Non=annotation('textbox',Non_dim,'String',Non_str,'FitBoxToText','on');
Non.Color='red';

%% Markov textbox
dim = [x .6 .3 .3];
%dim = [.6 .1 .3 .3];
str = {'Markov Solution:',Markov_RiseTime,Markov_SettlingTime,Markov_SettlingMin,Markov_SettlingMax,Markov_Overshoot,Markov_Undershoot,Markov_Peak}
Non=annotation('textbox',dim,'String',str,'FitBoxToText','on');
Non.Color='blue';
