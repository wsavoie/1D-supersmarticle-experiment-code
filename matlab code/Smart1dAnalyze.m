% clear all;
% close all;
pickFile=1;

if pickFile
    fold=uigetdir('A:\1DSmartData\Optitrack\');
    load(fullfile(fold,'movieInfo.mat'));
else
    load('A:\1DSmartData\Optitrack\data 2017-04-26\movieInfo.mat');
end
% figure(1)
fullfile(fold,'movieInfo.mat')

%************************************************************
%* Fig numbers:
%* 1. check names of markers
%* 2. plot displacement of frame
%* 3. plot drift for different relative phases
%* 4. plot mean speed vs time
%* 5. plotting location of open area
%* 6. plot speed of frame vs speed smart b
%* 7. distance per cycle
%* 8. plot drift speed vs mode (wheel mode displacement)
%* 9.
%*10.
%*11.
%*12. 
%*13/14 plot yoke rotation for both smarticles
%************************************************************
showFigs=[1 13];

%define curve params [] for all
state=[]; phase1=[]; phase2=[]; v=[]; m=[]; f=[];
%state=10 is for testing



props={state phase1 phase2 v};
% props={state phase1 phase2 v m};
% props={state phase1 phase2 v m f};
inds=1;
for i=1:length(movs)
    
    cond=true;
    for j=1:length(props)
        %if empty accept all values
        if ~isempty(props{j})
            %in case multiple numbers in property
            %if no matches then cond = false
            if(~any(props{j}==movs(i).pars(j)))
                cond = false;
            end
        end
    end
    if(cond)
        %         ma = ma.addAll(movs(i).data(1));
        usedMovs(inds)=movs(i);
        inds=inds+1;
    end
end
if(isempty(usedMovs))
    error('no tracks found for params given!');
end
pars=[usedMovs(:).pars];


if length(props)==4
    props={state phase1 phase2 v};
elseif length(props)==5
    props={state phase1 phase2 v m};
elseif length(props)==6
    props={state phase1 phase2 v m f};
end
pars=reshape(pars,[length(props),numel(pars)/length(props)])';
N=length(usedMovs);
% try
%     pars=reshape(pars,[length(props),numel(pars)/length(props)])';
%
% catch
%     pts('this is old code, am not using mode specifier!');
%     props={state phase1 phase2 v};
%     pars=reshape(pars,[length(props),numel(pars)/length(props)])';
%     N=length(usedMovs);
%     %     error('don''t use mode for this old code or redo it')
% end
%% 1 check names of markers
xx=1;
if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
    
    %     plot(usedMovs(1).x(:,1),usedMovs(1).y(:,1),'o-');
    %     plot(usedMovs(1).x(:,2),usedMovs(1).y(:,2),'o-');
    %     plot(usedMovs(1).x(:,3),usedMovs(1).y(:,3),'o-');
    %      plot(usedMovs(1).x(:,4),usedMovs(1).y(:,4),'o-');
    %           plot(usedMovs(1).x(:,5),usedMovs(1).y(:,5),'o-');
    axis equal
    
    plot(usedMovs(1).rots(:,1),usedMovs(1).rots(:,3),'o-');
    plot(usedMovs(1).rots(:,2),usedMovs(1).rots(:,4),'o-');
    
    plot(usedMovs(1).frame(:,1),usedMovs(1).frame(:,2),'s');
    
    
    plot(usedMovs(1).smarts(:,1),usedMovs(1).smarts(:,3),'^-');
    plot(usedMovs(1).smarts(:,2),usedMovs(1).smarts(:,4),'^-');
    legend({'rot a','rot b', 'frame','smart a', 'smart b'});
end
%% 2 plot displacement of frame
xx=2;
vv=[];
if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
    for i=1:length(usedMovs)
        plot(usedMovs(i).frame(:,1)+i,usedMovs(i).frame(:,2),'s');
        vv{i}=['v',num2str(usedMovs(i).pars(4))];
    end
    legend(vv);
end
%% 3 plot drift for different relative phases
xx=3;
if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
    xlim([-1.1,1.1]);
    uniPhase=unique(pars(:,2)-pars(:,3));
    drifts=cell(length(uniPhase),1);
    for j=1:length(uniPhase)
        for i=1:length(usedMovs)
            movPhase= -diff(usedMovs(i).pars(2:3));
            if movPhase==uniPhase(j)
                drift=(usedMovs(i).frame(end,2)-usedMovs(i).frame(1,2))/...
                    usedMovs(i).t(end)*1000;
                drifts{j}=[drifts{j} drift];
            end
        end
        plot(ones(length(drifts{j}),1)*uniPhase(j),[drifts{j}],'o')
    end
    driftMean=cellfun(@mean,drifts)
    errorbar(uniPhase,cellfun(@mean,drifts),cellfun(@std,drifts),'k')
    figText(gcf,16);
    axis equal
    xlabel('(\phi_1-\phi_2)/(\pi)');
    ylabel('drift speed (mm/s)');
    
end
%% 4  plot mean speed vs time
xx=4;
if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
    i=1;
    v=cell(N,1);
    %     for i=1:N
    [pks locs]=findpeaks(usedMovs(i).rots(:,1),'minpeakdistance',100);
    ypeakpos=usedMovs(i).frame(locs,2);
    t=usedMovs(i).t(locs);
    vel=diff(ypeakpos)./diff(t)*1000;
    v{i}=vel;
    plot(cumsum(diff(t)),vel);
    vv{i}=['v',num2str(usedMovs(i).pars(4)),' v=',num2str(mean(vel),'%2.2f')];
    %     end
    ylabel('velocity')
    xlabel('time(s)')
    legend(vv);
    figText(gcf,16);
end
%% 5 plotting location of open area
xx=5;
if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
end
%% 6 plot speed of frame vs speed smart b
xx=6;

if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
    i=1;
    t=usedMovs(i).t;
    c = linspace(1,10,length(t));
    %     for i=1:length(usedMovs)
    ibeg= 1;
    
    colormap fire;
    caxis([t(ibeg),t(end)])
    
    c=fire(length(usedMovs(i).t(ibeg:end))-1);
    caxis([t(ibeg),t(end)])
    
    subplot(1,2,1);
    %             plot(diff(usedMovs(i).smarts(ibeg:end,2))./diff(t(ibeg:end))*1000,1000*diff(usedMovs(i).frame(ibeg:end,2))./diff(t(ibeg:end)),'-');
    scatter(diff(usedMovs(i).smarts(ibeg:end,3))./diff(t(ibeg:end))*1000,1000*diff(usedMovs(i).frame(ibeg:end,2))./diff(t(ibeg:end)),[],c);
    %             comet(diff(usedMovs(i).smarts(ibeg:end,2))./diff(t(ibeg:end))*1000,1000*diff(usedMovs(i).frame(ibeg:end,2))./diff(t(ibeg:end)),.00001);
    xlabel(['s_1 ,speed (mm/s)']);
    ylabel('frame speed (mm/s)')
    
    subplot(1,2,2)
    %             plot(diff(usedMovs(i).smarts(ibeg:end,4))./diff(t(ibeg:end))*1000,1000*diff(usedMovs(i).frame(ibeg:end,2))./diff(t(ibeg:end)),'-');
    scatter(diff(usedMovs(i).smarts(ibeg:end,4))./diff(t(ibeg:end))*1000,1000*diff(usedMovs(i).frame(ibeg:end,2))./diff(t(ibeg:end)),[],c);
    xlabel(['s_2 speed (mm/s)']);
    caxis([t(ibeg),t(end)])
    cc=colorbar;
    figText(gcf,16);
    %     end
end

%% 7 distance per cycle
xx=7;
if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
    
    i=1;
    
    d=cell(N,1);
    lens=zeros(N,1);
    for i=1:N
        [pks,locs]=findpeaks(usedMovs(i).rots(:,1),'minpeakdistance',100);
        ypeakpos=usedMovs(i).frame(locs,2);
        t=usedMovs(i).t(locs);
        d{i}=diff(ypeakpos)*1000;
        lens(i)=length(ypeakpos)-1;
        
        %         vel=diff(ypeakpos)./diff(t)*1000;
        %         v{i}=vel;
        %         plot(cumsum(diff(t)),vel);
        %         vv{i}=['v',num2str(usedMovs(i).pars(4)),' v=',num2str(mean(vel),'%2.2f')];
    end
    dat=zeros(min(lens),N);
    for(j=1:N)
        dat(:,j)=d{j}(1:min(lens));
    end
    errorbar(mean(dat,2),std(dat,0,2),'o-');
    
    %     plot(d{i}(,'o-');
    xlabel('period number')
    ylabel('displacement (mm)')
    figText(gcf,16);
end
%% 8 plot drift speed vs mode (wheel mode displacement)
xx=8;
if(showFigs(showFigs==xx))
    if(size(pars,2)< 5)
        error('too few pars, plot 8 speed vs mode, won''t work');
    end
    
    figure(xx)
    hold on;
    xmin=-2.1; xmax=2.1;
    axis([xmin,xmax,-0.35,0.35]);
    unimode=unique(pars(:,5));
    drifts=cell(length(unimode),1);
    
    for j=1:length(unimode)
        for i=1:length(usedMovs)
            movMode =(usedMovs(i).pars(5));
            if movMode==unimode(j)
                %                 t=find(usedMovs(i).t>3.0,1);
                t=1;
                
                drift=(usedMovs(i).frame(end,2)-usedMovs(i).frame(t,2))/...
                    usedMovs(i).t(end)*1000;
                drifts{j}=[drifts{j} drift];
            end
        end
        plot(ones(length(drifts{j}),1)*unimode(j),[drifts{j}],'o')
    end
    driftMean=cellfun(@mean,drifts)
    errorbar(unimode,cellfun(@mean,drifts),cellfun(@std,drifts),'k')
    plot([xmin,xmax],[0,0],'r','linewidth',1.2);
    figText(gcf,16);
    %     axis equal
    xlabel('mode');
    ylabel('drift speed (mm/s)');
    
end
%% 9
xx=9;
if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
end
%% 10
xx=10;
if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
end
%% 11
xx=11;
if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
end

%% 12
xx=12;
if(showFigs(showFigs==xx))
    figure(xx)
    hold on;
end

%% 13 plot yoke rotation for both smarticles
xx=13;
if(showFigs(showFigs==xx))
    figure(xx);
    hold on;
    x=1;
    t=(usedMovs(x).t<10);
    %         rotdisa=sum(sqrt(diff(usedMovs(1).rots(t,1)).^2+diff(usedMovs(1).rots(t,3)).^2))
    %         rotdisb=sum(sqrt(diff(usedMovs(1).rots(t,2)).^2+diff(usedMovs(1).rots(t,4)).^2))
    %         dx=diff(usedMovs(1).rots(t,1)*1000);
    %         dy=diff(usedMovs(1).rots(t,3)*1000);
    %         int = 1+(dy./dx).^2;
    %         len=sum(int.*dx);
    sx=usedMovs(x).smarts(t,1:2);
    sy=usedMovs(x).smarts(t,3:4);
    rx=usedMovs(x).rots(t,1:2);
    ry=usedMovs(x).rots(t,3:4);
    rz=zeros(size(ry));
    
    sx=bsxfun(@minus,sx,sx(1,:));
    sy=bsxfun(@minus,sy,sy(1,:));
    rx=bsxfun(@minus,rx,rx(1,:));
    ry=bsxfun(@minus,ry,ry(1,:));
    newrx=rx-sx; 
    newry=ry-sy;
    
    newrx=bsxfun(@minus,newrx,min((newrx)));
    newry=bsxfun(@minus,newry,min((newry)));
    
%     newrx=bsxfun(@minus,newrx,max(abs(newrx)/2));
    newry=bsxfun(@minus,newry,max(abs(newry)/2));
    newrx=bsxfun(@rdivide,newrx,max(abs(newrx)));
    newry=bsxfun(@rdivide,newry,max(abs(newry)));
    
%     zz=[newrx(:,1)'; newry(:,1)'];
%     rot=4*pi/180;
%     r=[cos(rot) -sin(rot); sin(rot) cos(rot)]
%     zz=(r*zz)';
%        plot(zz(:,1),zz(:,2),'k'); 
    plot(newrx(:,1),newry(:,1));

    plot(newrx(:,2),newry(:,2));
%     phi=atan2(newry,newrx)/pi*180;
    phi=atan2(newry,newrx);
%     plot(rx1,newry(:,2));
%     plot(rx2,newry(:,2));
    axis tight
    axis equal
    figText(gcf,16); 
    title('yoke trajectory');
    xlabel('x (normalized)'); ylabel('y (normalized)');
    
    figure(xx+1);
    hold on;
    plot(phi)
    plot(sin(phi));
    figText(gcf,16); 
    xlabel('frame'); ylabel('\theta');
    legend({'smarticle 1','smarticle 2'})
    
end
%% 15 
xx=15;
if(showFigs(showFigs==xx))
    figure(xx);
    hold on;
end