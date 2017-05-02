figure(1);
hold on;
file='A:\1DSmartData\ContactData\-pi2_s=1_v=1.txt';
dat = importdata(file);
ylim([1,7]);
states=dat(:,1);
t=dat(:,2);
% states(diff(states)==0)=[];
newstate=states;
for i=1:length(newstates);

end
stairs(t(t<10),states(t<10));
figText(gcf,16);
%% state transitions
nonRepStates = states(diff([0; states])~=0);
stateT=[
    1,1;1,2;1,5;...
    2,1;2,2;2,5;2,6;2,7;...
    3,3;3,4;3,5;...
    4,3;4,4;4,6;4,7;...
    5,1;5,3;5,5;5,6;...
    6,2;6,4;6,5;6,6;6,7;...
    7,2;7,4;7,6;7,7];
trans=zeros(size(stateT,1),1);
% stateTrans(:,1:2)=stateT;

for i=1:size(nonRepStates,1)-1
    temp=[nonRepStates(i),nonRepStates(i+1)];
    [~,ind]=intersect(stateT,temp,'rows');
    trans(ind)=trans(ind)+1;
    
end
% % importdata(uigetfile({'*.txt'},'A:\1DSmartData\ContactData'));
% phase='\pi/2';
% nPeriods=29;
% figure(1);
% hold on;
% cols=get(gca,'colororder');
% axis([0.5,7.5,0 framef(nPeriods)])
% 
% for(i=1:nPeriods)
% rectangle('position',[state(i)-0.5,framei(i),1,framef(i)-framei(i)],'facecolor',cols(state(i),:))
% end
% % plot(state(1:nPeriods),framei(1:nPeriods),'k','linewidth',2);
% xlabel('state');
% ylabel('frames');
% title(phase);
% figText(gcf,18);
% %%
% 
% figure(2);
% hold on;
% axis([0 framef(nPeriods) 0.5,7.5])
% for(i=1:nPeriods)
% rectangle('position',[framei(i),state(i)-0.5,framef(i)-framei(i),1],'facecolor',cols(state(i),:))
% end
% % plot(framei(1:nPeriods),state(1:nPeriods),'k','linewidth',2);
% xlabel('frames');
% ylabel('state');
% title(phase);
% figText(gcf,18);
% 
% %%
% figure(3);
% hold on;
% 
% axis([0,1,0 framef(nPeriods)])
%     % for(i=1:length(state)-1)
% for(i=1:nPeriods)
% rectangle('position',[0,framei(i),1,framei(i+1)-framei(i)],'facecolor',cols(state(i),:))
% end
% set(gca,'xtick',[],'xticklabel',[]);
% xlabel('');
% 
% ylabel('frames');
% title(phase);
% figText(gcf,18);
% 
% %%
% figure(4);
% hold on;
% statetime=zeros(1,framef(nPeriods));
% period=1;
% for i=1:framef(nPeriods)
%     if(i>=framef(period+1))
%         period=period+1;
%     end
%     statetime(i)=state(period);
% end
% plot(1:framef(nPeriods),statetime,'k-','linewidth',2);
% xlabel('frames');
% ylabel('period');
% title(phase);
% axis tight
% figText(gcf,18);