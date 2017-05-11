function [t, modes ]=trackContacts(file)
dat = importdata(fullfile(file));
t=dat(:,1);
modes=dat(:,2);