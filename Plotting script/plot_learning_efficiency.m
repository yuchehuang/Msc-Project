clear
%Loading txt file dir default Desktop
Non_Markov_address='Markov balancing Reward.txt';

Non_data=dlmread(Non_Markov_address);
Non_Size= size(Non_data);


%% Fast Average line 
%create a array with same length of data 
Non_Average(Non_Size(1))=0;
total_value=0;
for length=1:Non_Size(1)
    total_value=total_value+Non_data(length,2);
    Non_Average(1,length)=total_value/length;
end

%% Fast Highest value tracking
NOn_highest(Non_Size(1))=0; 
Non_Max_value=Inf;
for length=1:Non_Size(1)
    if(Non_data(length,2)<Non_Max_value)
        Non_Max_value=Non_data(length,2); 
    end
    NOn_highest(1,length)=Non_Max_value;
end

%% Plot all data
figure(1);
%plot(-Non_data(:,2))
hold on
plot(-NOn_highest(1,:),'r','LineWidth',1.5)
plot(-Non_Average(1,:),'LineWidth',1.5)
%plot(-Lowest(1,:),'y')
hold off
xlabel('Episode'); 
ylabel('Reward');
title(Non_Markov_address);
legend('Best reward','Average')