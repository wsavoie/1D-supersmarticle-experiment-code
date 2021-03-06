% load('states.mat');
phase='\phi_{offset}=-\pi/2 ';
nPeriods=29;
figure(1);
hold on;
cols=get(gca,'colororder');
axis([0.5,7.5,0 framef(nPeriods)])

for(i=1:nPeriods)
rectangle('position',[state(i)-0.5,framei(i),1,framef(i)-framei(i)],'facecolor',cols(state(i),:))
end
% plot(state(1:nPeriods),framei(1:nPeriods),'k','linewidth',2);
xlabel('mode');
ylabel('frames');
title(phase);
figText(gcf,18);
%%

figure(2);
hold on;
axis([0 framef(nPeriods) 0.5,7.5])
for(i=1:nPeriods)
rectangle('position',[framei(i),state(i)-0.5,framef(i)-framei(i),1],'facecolor',cols(state(i),:))
end
% plot(framei(1:nPeriods),state(1:nPeriods),'k','linewidth',2);
xlabel('frames');
ylabel('mode');
title(phase);
figText(gcf,18);

%%
figure(3);
hold on;

axis([0,1,0 framef(nPeriods)])
    % for(i=1:length(state)-1)
for(i=1:nPeriods)
rectangle('position',[0,framei(i),1,framei(i+1)-framei(i)],'facecolor',cols(state(i),:))
end
set(gca,'xtick',[],'xticklabel',[]);
xlabel('');

ylabel('frames');
title(phase);
figText(gcf,18);

%%
figure(4);
hold on;
statetime=zeros(1,framef(nPeriods));
period=1;
for i=1:framef(nPeriods)
    if(i>=framef(period+1))
        period=period+1;
    end
    statetime(i)=state(period);
end
plot(1:framef(nPeriods),statetime,'k-','linewidth',2);
xlabel('frames');
ylabel('period');
title(phase);
axis tight
figText(gcf,18);