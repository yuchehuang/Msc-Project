clear

%% The default folder for reading external file is in Desktop
MakKov='F=100 L=65 Markov SwingUp StateLog.txt';
Nom_MakKov='F=100 L= 57 Non Markov  SwingUP StateLog.txt';
MakKov_data=dlmread(MakKov);
MakKov_Size= size(MakKov_data);

Nom_MakKov_data=dlmread(Nom_MakKov);
Nom_MakKov_Size= size(Nom_MakKov_data);

y_ten_degree= 10*pi/180;    %giving the term position of the pendulum agnle

t=(0.01:0.01:0.65);         
T=(0.01:0.01:0.57);

if MakKov_Size(1)>Nom_MakKov_Size(1)
    x_length=MakKov_Size(1)/100;
else
     x_length=Nom_MakKov_Size(1)/100;
end

%% Select the angle of second pendulum
col=2; 

%% Scan when the algorithm operate the pendulum within 10 degree (Nom_MakKov)
for i=1:Nom_MakKov_Size(1)
    if abs(Nom_MakKov_data(i,col))<y_ten_degree
        Non_Markov_ten=i;
        break
    end
end

%% Scan when the algorithm operate the pendulum within 10 degree (MakKov)
for i=1:MakKov_Size(1)
    if abs(MakKov_data(i,col))<y_ten_degree
        Markov_ten=i;
        break
    end
end


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
SettlingTimeThreshold=0.03;
disp('MakKov_Sys:')
Markov_result=stepinfo(MakKov_data(:,col),t,'SettlingTimeThreshold',SettlingTimeThreshold)
Markov_x=Markov_result.SettlingTime;
Markov_y=MakKov_data(fix(100*Markov_x),col);

disp('=========================')
disp('Non MakKov_Sys:')
Non_Markov_result=stepinfo(Nom_MakKov_data(:,col),T,'SettlingTimeThreshold',SettlingTimeThreshold)
Non_Markov_x=Non_Markov_result.SettlingTime;
Non_Markov_y=Nom_MakKov_data(fix(100*Non_Markov_x),col);


%% Plot all data
figure(1);

plot(t,MakKov_data(:,col),'LineWidth',1)

hold on
line([Markov_ten/100 Markov_ten/100], [MakKov_data(Markov_ten,col) -3.5],'Color','Blue','LineStyle','--');
line([Non_Markov_ten/100 Non_Markov_ten/100], [Nom_MakKov_data(Non_Markov_ten) -3.5],'Color','red','LineStyle','--');
plot(T,Nom_MakKov_data(:,col),'r','LineWidth',1)
line([0 x_length], [-y_ten_degree -y_ten_degree],'Color','Black','LineStyle','--');
%plot(abs(Nom_MakKov_data(:,col)),'r','LineWidth',1)
%plot(LPF(1,:),'LineWidth',1)
%plot(Non_LPF(1,:),'r','LineWidth',1)
hold off
ylabel('Theta 2 (rad)')
xlabel('Time (sec)');
title('-100 <cart Force<100    SwingUp task');
legend('MarKov','Markov SettlingTime','Non Markov SettlingTime','Non-MarKov')



