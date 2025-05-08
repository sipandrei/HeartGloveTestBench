apasari = linspace(0,0,31);
a = linspace(11,11,31);
pozitie = linspace(0,31,31);
disp(citiri);

stderror = std(citiri{:,2})/sqrt(length(citiri{:,2}));
readingMean = mean(citiri{:,2});
absError = abs(abs(citiri{:,2}-a));
meanRelError = abs(citiri{:,2}-a)./a;
meanRelErrorPer = meanError*100;
meanError = mean(absError*100/a(1,1));


disp(["Valoare medie citita: ",num2str(readingMean)]);
disp(["Eroare medie: ",num2str(meanError(1,1))]);
disp(["Abatere medie patratica: ", num2str(stderror)]);
disp(["Abatere medie patratica procentaj: ", num2str(stderror*100/a(1,1))]);

figure;
subplot(2,1,1);
plot(pozitie,a, "-b");
hold on;
plot(pozitie,linspace(readingMean,readingMean,31),color=[0 0.75 0.25]);
scatter(pozitie,citiri{:,2},".r");
title("Deplasarea citită față de deplasarea țintă");
xlabel("Citire");
ylabel("Deplasare");
grid on;
axis([0,32,0,20]);
legend("Deplasare tinta","Deplasare medie citita","Valori citite", Location="best");
hold off;

subplot(2,1,2);
plot(pozitie,meanErrorPer(:,1),Color=[0 0.5 0.5]);
hold on;
axis([0,32,0,100]);
xlabel("Citire");
ylabel("Percentage");
title("Eroarea medie absoluta(procentual)")
hold off;