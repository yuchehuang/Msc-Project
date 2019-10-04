clear
%Loading txt file dir default Desktop
Markov_address='Markov Balancing Reward.txt';
Non_Markov_address='Non Markov Balancing Reward.txt';

Markov_data=dlmread(Markov_address);
Markov_Size= size(Markov_data);

Non_Markov_data=dlmread(Non_Markov_address);
Non_Markov_Size= size(Non_Markov_data);

%% Fast Average line 
%create a array with same length of data 
Markov_Average(Markov_Size(1))=0;
Markov_total_value=0;
for length=1:Markov_Size(1)
    Markov_total_value=Markov_total_value+Markov_data(length,2);
    Markov_Average(1,length)=Markov_total_value/length;
end

%% Fast Average line 
%create a array with same length of data 
Non_Markov_Average(Non_Markov_Size(1))=0;
Non_Markov_total_value=0;
for length=1:Non_Markov_Size(1)
    Non_Markov_total_value=Non_Markov_total_value+Non_Markov_data(length,2);
    Non_Markov_Average(1,length)=Non_Markov_total_value/length;
end

%% Fast Highest value tracking
Markov_highest(Markov_Size(1))=0; 
Markov_Max_value=Inf;
for length=1:Markov_Size(1)
    if(Markov_data(length,2)<Markov_Max_value)
        Markov_Max_value=Markov_data(length,2); 
    end
    Markov_highest(1,length)=Markov_Max_value;
end


%% Fast Highest value tracking
Non_Markov_highest(Non_Markov_Size(1))=0; 
Non_Markov_Max_value=Inf;
for length=1:Non_Markov_Size(1)
    if(Non_Markov_data(length,2)<Non_Markov_Max_value)
        Non_Markov_Max_value=Non_Markov_data(length,2); 
    end
    Non_Markov_highest(1,length)=Non_Markov_Max_value;
end

%% Plot all data
figure(1);
%plot(-Markov_data(:,2))
hold on

plot(-Markov_highest(1,:),'b','LineWidth',1.5)
plot(-Non_Markov_highest(1,:),'r','LineWidth',1.5)
plot(-Markov_Average(1,:),'b:','LineWidth',1.5)
plot(-Non_Markov_Average(1,:),'r:','LineWidth',1.5)
%plot(-Lowest(1,:),'y')
hold off
xlabel('Episode'); 
ylabel('Reward');
title(Non_Markov_address);
legend('Markov-Highest','Non-Markov-Highest','Markov-average','Non-Markov-average')