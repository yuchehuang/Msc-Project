clear
length= 100
c1(1:length)=0;
c2(1:length)=0;
c3(1:length)=0;
c4(1:length)=0;
c5(1:length)=0;
c6(1:length)=0;
t= (-length+1:0);

for i= 1:length
    temp=1;
    for j=i:length
        temp=temp*0.99;
    end
    c1(1,i)=temp;
end

for i= 1:length
    temp=1;
    for j=i:length
        temp=temp*0.95;
    end
    c2(1,i)=temp;
end

for i= 1:length
    temp=1;
    for j=i:length
        temp=temp*0.9;
    end
    c3(1,i)=temp;
end

for i= 1:length
    temp=1;
    for j=i:length
        temp=temp*0.85;
    end
    c4(1,i)=temp;
end

for i= 1:length
    temp=1;
    for j=i:length
        temp=temp*0.8;
    end
    c5(1,i)=temp;
end

plot(t,c1(1,:),'LineWidth',2);
hold on
plot(t,c2(1,:),'LineWidth',2);
plot(t,c3(1,:),'LineWidth',2);
plot(t,c4(1,:),'LineWidth',2);
plot(t,c5(1,:),'LineWidth',2);
xlabel('number of delay time'); 
ylabel('Weight amptitude');
title('Weight characteristic by using different discount factor');
legend('r=0.99','r=0.95','r=0.9','r=0.85','r=0.8')