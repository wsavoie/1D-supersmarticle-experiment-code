figure(2);
hold on;
fold='A:\1DSmartData\ContactData\';
file='s=1_pa=0.5_pb=0_v=4.txt';
[~,nums]=parseFileNames(file);
phase=nums(2)-nums(3);

dat = importdata(fullfile(fold,file));
ylim([0.9,7.1]);
modes=dat(:,1);
t=dat(:,2);
% modes(diff(modes)==0)=[];
% newmode=modes;
% for i=1:length(newmodes);
% 
% end
tend=10;
stairs(t(t<tend),modes(t<tend));
% stairs(t,modes);
figText(gcf,16);
xlabel('time (s)');
ylabel('modes');
title(['\Delta\phi=',num2str(phase),'\pi']);

% %% mode transitions
% nonRepmodes = modes(diff([0; modes])~=0);
% modeT=[
%     1,1;1,2;1,5;...
%     2,1;2,2;2,5;2,6;2,7;...
%     3,3;3,4;3,5;...
%     4,3;4,4;4,6;4,7;...
%     5,1;5,3;5,5;5,6;...
%     6,2;6,4;6,5;6,6;6,7;...
%     7,2;7,4;7,6;7,7];
% trans=zeros(size(modeT,1),1);
% % modeTrans(:,1:2)=modeT;
% 
% for i=1:size(nonRepModes,1)-1
%     temp=[nonRepModes(i),nonRepModes(i+1)];
%     [~,ind]=intersect(modeT,temp,'rows');
%     trans(ind)=trans(ind)+1;
%     
% end
% % importdata(uigetfile({'*.txt'},'A:\1DSmartData\ContactData'));
% phase='\pi/2';
% nPeriods=29;
% figure(1);
% hold on;
% cols=get(gca,'colororder');
% axis([0.5,7.5,0 framef(nPeriods)])
% 
% for(i=1:nPeriods)
% rectangle('position',[mode(i)-0.5,framei(i),1,framef(i)-framei(i)],'facecolor',cols(mode(i),:))
% end
% % plot(mode(1:nPeriods),framei(1:nPeriods),'k','linewidth',2);
% xlabel('mode');
% ylabel('frames');
% title(phase);
% figText(gcf,18);
% %%
% 
% figure(2);
% hold on;
% axis([0 framef(nPeriods) 0.5,7.5])
% for(i=1:nPeriods)
% rectangle('position',[framei(i),mode(i)-0.5,framef(i)-framei(i),1],'facecolor',cols(mode(i),:))
% end
% % plot(framei(1:nPeriods),mode(1:nPeriods),'k','linewidth',2);
% xlabel('frames');
% ylabel('mode');
% title(phase);
% figText(gcf,18);
% 
% %%
% figure(3);
% hold on;
% 
% axis([0,1,0 framef(nPeriods)])
%     % for(i=1:length(mode)-1)
% for(i=1:nPeriods)
% rectangle('position',[0,framei(i),1,framei(i+1)-framei(i)],'facecolor',cols(mode(i),:))
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
% modetime=zeros(1,framef(nPeriods));
% period=1;
% for i=1:framef(nPeriods)
%     if(i>=framef(period+1))
%         period=period+1;
%     end
%     modetime(i)=mode(period);
% end
% plot(1:framef(nPeriods),modetime,'k-','linewidth',2);
% xlabel('frames');
% ylabel('period');
% title(phase);
% axis tight
% figText(gcf,18);