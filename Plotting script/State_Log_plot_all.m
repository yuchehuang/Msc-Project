clear
%% Loading txt file dir default Desktop
address='F=100 L= 57 Non Markov  SwingUP StateLog.txt';
data=dlmread(address);
Size= size(data);
t=(0.01:0.01:Size(1)/100);
%col=1;

%% Low pass Filter 
% Temp=data(:,col)';
% LPF_parameter=[0.5 0.5];
% LPF= conv2(LPF_parameter,Temp);
% % Resize the matrix due to the LPF will increase the length of the martix 
% LPF =LPF(1,1:Size);

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
%% Average line 
% %create a array with same length of data 
% Average(Size(1))=0; 
% for length=1:Size(1)
%     for element=1:length
%        Average(1,length)=Average(1,length)+data(element,2);
%     end
%     Average(1,length)=Average(1,length)/length;
% end

% %% Highest value tracking
% highest(Size(1))=0; 
% for length=1:Size(1)
%     Max_value=Inf; 
%     for element=1:length
%        if(data(element,2)<Max_value)
%             highest(1,length)=data(element,2);
%             Max_value=data(element,2);
%        end
%     end
% end

%% recently Lowest value tracking
% Lowest(Size(1))=0; 
% for length=1:Size(1)
%     Min_value=-Inf; 
%     if length<= 10
%         for element=1:length
%             if(data(element,2)>Min_value)
%                 Lowest(1,length)=data(element,2);
%                 Min_value=data(element,2);
%             end
%         end      
%     else
%         for element=length-10:length
%             if(data(element,2)>Min_value)
%                 Lowest(1,length)=data(element,2);
%                 Min_value=data(element,2);
%             end
%         end      
%     end    
% end

%% Plot all data
figure(1);

for i=2:3
    subplot(3,1,i)
    plot(t,data(:,i-1),'LineWidth',1.5)
    hold on
    plot(t,data(:,i),'r','LineWidth',1.5)
    hold off
    ylabel('Theta (rad)');
    legend('Pendulum 1','Pendulum 2')
end

subplot(3,1,1)
plot(t,data(:,5),'LineWidth',1.5)
ylabel('Force (Nt)');
xlabel('Time (sec)'); 
legend('Pendulum 1','Pendulum 2')



% 
% plot(data(:,col),'LineWidth',1)
% %plot(abs(data(:,col)),'LineWidth',1)
% 
% %plot(-Average(1,:),'g','LineWidth',1)
% %plot(-highest(1,:),'r','LineWidth',1.5)
% %plot(-Lowest(1,:),'y')
% %plot(LPF(1,:),'g','LineWidth',1)
% hold off
% xlabel('Time (10 milli sec)'); 
% ylabel('Theta (rad)');
% title(address);
% legend('Reward','LPF',' Highest')