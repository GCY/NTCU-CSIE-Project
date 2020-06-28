#include <iostream>
#include <cmath> // tanh用
#include <iomanip> // 四捨五入用
using namespace std;

int main()
{
    double leftElbow = 0; // 左肘
    double leftShoulder = 0; // 左肩
    double rightElbow = 0; // 右肘
    double rightShoulder = 0; // 右肩
    
    double scaled_leftElbow = 0;
    double scaled_leftShoulder = 0;
    double scaled_rightElbow = 0;
    double scaled_rightShoulder = 0;
    
    double neural[40]; // 40個節點
    double action[5]; // 5個輸出
    
    
    cout << "Enter angles(左肘 左肩 右肘 右肩): ";
    cin >> leftElbow >> leftShoulder >> rightElbow >> rightShoulder;
    
    scaled_leftElbow = 2*(leftElbow-74.4002)/(289.601-74.4002)-1;
    scaled_leftShoulder = 2*(leftShoulder-0.0363245)/(59.8926-0.0363245)-1;
    scaled_rightElbow = 2*(rightElbow-69.5387)/(224.504-69.5387)-1;
    scaled_rightShoulder = 2*(rightShoulder-0.0891446)/(62.7631-0.0891446)-1;
    
    neural[0] = tanh(3.80831+0.52486*scaled_leftElbow+0.162072*scaled_leftShoulder-1.46459*scaled_rightElbow+1.55418*scaled_rightShoulder);
    neural[1] = tanh(4.14283-0.694094*scaled_leftElbow+0.090494*scaled_leftShoulder-2.71232*scaled_rightElbow+1.63493*scaled_rightShoulder);
    neural[2] = tanh(2.25335+1.38785*scaled_leftElbow+1.13202*scaled_leftShoulder-5.08136*scaled_rightElbow+1.26395*scaled_rightShoulder);
    neural[3] = tanh(-0.11931+2.01193*scaled_leftElbow-4.14088*scaled_leftShoulder+3.20298*scaled_rightElbow-2.36577*scaled_rightShoulder);
    neural[4] = tanh(0.486598+3.66794*scaled_leftElbow+2.34*scaled_leftShoulder+6.70782*scaled_rightElbow+11.1351*scaled_rightShoulder);
    neural[5] = tanh(2.05973+1.0241*scaled_leftElbow-0.574356*scaled_leftShoulder-5.06302*scaled_rightElbow+2.74653*scaled_rightShoulder);
    neural[6] = tanh(3.52765-1.59476*scaled_leftElbow+0.38336*scaled_leftShoulder-0.985025*scaled_rightElbow-0.136948*scaled_rightShoulder);
    neural[7] = tanh(-18.9167-5.21035*scaled_leftElbow-13.5971*scaled_leftShoulder+8.48343*scaled_rightElbow-6.64924*scaled_rightShoulder);
    neural[8] = tanh(-3.65292-0.548621*scaled_leftElbow-0.730578*scaled_leftShoulder+3.47095*scaled_rightElbow+0.395476*scaled_rightShoulder);
    neural[9] = tanh(5.14548+3.08272*scaled_leftElbow-2.84378*scaled_leftShoulder-2.26081*scaled_rightElbow+1.73607*scaled_rightShoulder);
    neural[10] = tanh(-6.84593+5.80158*scaled_leftElbow-1.80086*scaled_leftShoulder+1.63423*scaled_rightElbow-6.90244*scaled_rightShoulder);
    neural[11] = tanh(1.77403-0.482242*scaled_leftElbow+1.50333*scaled_leftShoulder-6.86996*scaled_rightElbow-3.35671*scaled_rightShoulder);
    neural[12] = tanh(-2.63469-7.18863*scaled_leftElbow-4.79091*scaled_leftShoulder+4.67011*scaled_rightElbow+5.07468*scaled_rightShoulder);
    neural[13] = tanh(3.37843+0.677297*scaled_leftElbow-0.665581*scaled_leftShoulder-0.756786*scaled_rightElbow-0.685613*scaled_rightShoulder);
    neural[14] = tanh(5.13623+1.28832*scaled_leftElbow-0.675797*scaled_leftShoulder-0.479302*scaled_rightElbow-1.14462*scaled_rightShoulder);
    neural[15] = tanh(1.85336+8.47066*scaled_leftElbow+0.164499*scaled_leftShoulder-4.60197*scaled_rightElbow+1.04498*scaled_rightShoulder);
    neural[16] = tanh(1.17974-1.52221*scaled_leftElbow-1.27397*scaled_leftShoulder-0.332222*scaled_rightElbow-3.13701*scaled_rightShoulder);
    neural[17] = tanh(12.7882-28.9913*scaled_leftElbow+12.2106*scaled_leftShoulder-1.59226*scaled_rightElbow-2.40194*scaled_rightShoulder);
    neural[18] = tanh(1.19112-2.10212*scaled_leftElbow-2.40668*scaled_leftShoulder-2.04249*scaled_rightElbow-5.4047*scaled_rightShoulder);
    neural[19] = tanh(-6.19531-2.99928*scaled_leftElbow-7.48341*scaled_leftShoulder+6.33644*scaled_rightElbow+4.83742*scaled_rightShoulder);
    neural[20] = tanh(-0.998562-7.62081*scaled_leftElbow-5.216*scaled_leftShoulder+2.54747*scaled_rightElbow+0.568535*scaled_rightShoulder);
    neural[21] = tanh(-3.05444+0.0133689*scaled_leftElbow+0.483002*scaled_leftShoulder+0.0433991*scaled_rightElbow-0.665607*scaled_rightShoulder);
    neural[22] = tanh(1.0916-1.48663*scaled_leftElbow-3.41124*scaled_leftShoulder+3.02446*scaled_rightElbow-1.67978*scaled_rightShoulder);
    neural[23] = tanh(-2.21186-2.7246*scaled_leftElbow-4.11963*scaled_leftShoulder+2.00652*scaled_rightElbow-1.10149*scaled_rightShoulder);
    neural[24] = tanh(-2.93161+1.13909*scaled_leftElbow-2.16185*scaled_leftShoulder+3.41217*scaled_rightElbow-1.36026*scaled_rightShoulder);
    neural[25] = tanh(-1.91762+7.43102*scaled_leftElbow-2.36321*scaled_leftShoulder-0.999153*scaled_rightElbow+1.14403*scaled_rightShoulder);
    neural[26] = tanh(2.45948+0.740925*scaled_leftElbow+0.609185*scaled_leftShoulder-0.476655*scaled_rightElbow-0.151576*scaled_rightShoulder);
    neural[27] = tanh(-3.79237-3.87466*scaled_leftElbow-1.30935*scaled_leftShoulder+1.70107*scaled_rightElbow+1.11954*scaled_rightShoulder);
    neural[28] = tanh(0.865779-10.7023*scaled_leftElbow+4.84313*scaled_leftShoulder+1.40772*scaled_rightElbow-2.21403*scaled_rightShoulder);
    neural[29] = tanh(-6.8573-2.99143*scaled_leftElbow-6.27242*scaled_leftShoulder+8.64162*scaled_rightElbow+1.52069*scaled_rightShoulder);
    neural[30] = tanh(5.36983-3.56599*scaled_leftElbow+3.80044*scaled_leftShoulder-7.56048*scaled_rightElbow-0.146492*scaled_rightShoulder);
    neural[31] = tanh(0.457771-3.00406*scaled_leftElbow+1.4711*scaled_leftShoulder+6.81434*scaled_rightElbow+1.30842*scaled_rightShoulder);
    neural[32] = tanh(-2.23851+3.82394*scaled_leftElbow-2.29394*scaled_leftShoulder+2.08864*scaled_rightElbow-0.464218*scaled_rightShoulder);
    neural[33] = tanh(6.0135+2.01509*scaled_leftElbow-1.06934*scaled_leftShoulder+0.253872*scaled_rightElbow-1.29893*scaled_rightShoulder);
    neural[34] = tanh(5.37846+3.52419*scaled_leftElbow-1.42533*scaled_leftShoulder+0.397833*scaled_rightElbow-0.46861*scaled_rightShoulder);
    neural[35] = tanh(1.33082+1.24317*scaled_leftElbow-0.406956*scaled_leftShoulder-1.55953*scaled_rightElbow-0.205749*scaled_rightShoulder);
    neural[36] = tanh(4.52175-13.3943*scaled_leftElbow+9.70813*scaled_leftShoulder+9.31619*scaled_rightElbow+1.06029*scaled_rightShoulder);
    neural[37] = tanh(2.15527-0.22829*scaled_leftElbow-1.86922*scaled_leftShoulder+2.5112*scaled_rightElbow-4.05274*scaled_rightShoulder);
    neural[38] = tanh(-4.35456+0.351659*scaled_leftElbow-3.59022*scaled_leftShoulder-2.83841*scaled_rightElbow-3.11791*scaled_rightShoulder);
    neural[39] = tanh(2.20281-1.21222*scaled_leftElbow-3.55868*scaled_leftShoulder+2.13479*scaled_rightElbow-1.36668*scaled_rightShoulder);

    /*
    cout << "--節點--" << endl;
    for(int i=0; i<4; i++){
        cout << "Neuron" << i << "_: ";
        for(int j=0; j<10; j++)
        {
             cout << neural[i*10+j];
            if(j=9)
                continue;
            cout << ", ";
        }
        cout << endl;
    }
    */
    
    cout << "--結果--" << endl;
    
    action[0] = (1.14679+2.95682*neural[0]-1.47219*neural[1]+0.703812*neural[2]-1.41529*neural[3]-0.47358*neural[4]+0.0436872*neural[5]+1.58528*neural[6]+0.1063*neural[7]-0.811046*neural[8]+4.29813*neural[9]-0.516858*neural[10]+0.110609*neural[11]-1.67141*neural[12]-2.2807*neural[13]+1.28203*neural[14]+2.43402*neural[15]+1.17227*neural[16]-2.23423*neural[17]+0.859717*neural[18]+1.66482*neural[19]-1.39021*neural[20]+0.406563*neural[21]+0.223442*neural[22]+2.10027*neural[23]-0.717852*neural[24]-5.01691*neural[25]-2.28296*neural[26]+1.80848*neural[27]-0.315626*neural[28]+0.852004*neural[29]-1.29034*neural[30]-0.370717*neural[31]-3.09782*neural[32]-2.77206*neural[33]-1.44326*neural[34]-0.222733*neural[35]-0.245605*neural[36]+0.717878*neural[37]-0.390834*neural[38]+0.546231*neural[39]);
    action[1] = (2.48653+1.37982*neural[0]+0.149168*neural[1]-0.178982*neural[2]+0.409597*neural[3]-0.0081389*neural[4]+0.00470992*neural[5]-0.478118*neural[6]+0.001263*neural[7]-0.16972*neural[8]+1.19956*neural[9]+0.0365916*neural[10]+0.0518496*neural[11]+0.0654062*neural[12]-4.71646*neural[13]+1.34462*neural[14]-0.0641592*neural[15]+0.304517*neural[16]-0.00476363*neural[17]-0.223772*neural[18]-0.0596114*neural[19]+0.0598252*neural[20]-2.27147*neural[21]-0.413062*neural[22]-0.063877*neural[23]-0.406724*neural[24]+0.134873*neural[25]-2.29261*neural[26]-1.34917*neural[27]+0.152024*neural[28]-0.0327272*neural[29]-0.0747621*neural[30]+0.0458067*neural[31]+0.326629*neural[32]-2.50679*neural[33]+0.481822*neural[34]-0.234601*neural[35]-0.0187858*neural[36]-0.952868*neural[37]-0.0281681*neural[38]+0.280049*neural[39]);
    action[2] = (-2.29713-2.48169*neural[0]-1.62251*neural[1]+0.814015*neural[2]-1.08494*neural[3]+0.568839*neural[4]-0.94239*neural[5]-3.57159*neural[6]-0.479613*neural[7]+1.03598*neural[8]+0.364848*neural[9]+0.018451*neural[10]-1.1553*neural[11]+2.40972*neural[12]-2.07902*neural[13]-0.84926*neural[14]-3.87643*neural[15]+0.648171*neural[16]+2.11222*neural[17]-2.43552*neural[18]-2.87685*neural[19]+0.550826*neural[20]-0.292267*neural[21]+0.477932*neural[22]-0.75161*neural[23]+0.91178*neural[24]+5.59578*neural[25]+0.359135*neural[26]-4.02019*neural[27]-0.241114*neural[28]-1.48502*neural[29]+1.49505*neural[30]-0.124927*neural[31]+2.72922*neural[32]+5.22187*neural[33]+1.37322*neural[34]+3.01096*neural[35]+0.509039*neural[36]-0.492298*neural[37]+1.15124*neural[38]+1.69597*neural[39]);
    action[3] = (-0.254625+0.0862541*neural[0]+4.16178*neural[1]-0.429646*neural[2]-1.27265*neural[3]-0.0715304*neural[4]+2.39959*neural[5]-1.53131*neural[6]+0.159698*neural[7]+2.86122*neural[8]-0.0415698*neural[9]+0.692481*neural[10]+0.618525*neural[11]-1.32363*neural[12]+2.91289*neural[13]-0.384719*neural[14]+2.91344*neural[15]-0.961162*neural[16]+0.686775*neural[17]+3.01143*neural[18]+2.20444*neural[19]+1.75282*neural[20]-0.514937*neural[21]-1.01829*neural[22]-3.18195*neural[23]+5.3963*neural[24]+0.430805*neural[25]-1.93189*neural[26]+0.744574*neural[27]+6.45409*neural[28]+1.29052*neural[29]-2.92615*neural[30]-2.63896*neural[31]+1.60856*neural[32]+0.394209*neural[33]+1.1742*neural[34]+2.15121*neural[35]-1.91366*neural[36]+0.219593*neural[37]-3.60471*neural[38]-0.973867*neural[39]);
    action[4] = (0.810649+1.47998*neural[0]-2.88132*neural[1]-0.627121*neural[2]+0.760332*neural[3]-0.0149457*neural[4]-1.97674*neural[5]+0.696496*neural[6]+0.177752*neural[7]-1.65686*neural[8]+0.99685*neural[9]-0.367475*neural[10]+0.263505*neural[11]+0.121006*neural[12]-1.14628*neural[13]-2.71086*neural[14]-1.10352*neural[15]-1.45082*neural[16]-0.411338*neural[17]-0.852244*neural[18]-0.524878*neural[19]-0.998451*neural[20]+0.266919*neural[21]+1.00701*neural[22]+1.72379*neural[23]-4.79462*neural[24]-1.38471*neural[25]+3.39769*neural[26]+0.180213*neural[27]-5.30691*neural[28]-0.382231*neural[29]+2.86887*neural[30]+2.75249*neural[31]-1.27419*neural[32]-1.30779*neural[33]-1.47324*neural[34]-1.69378*neural[35]+1.52462*neural[36]+0.070994*neural[37]+2.70424*neural[38]+0.910551*neural[39]);
    
    for(int i=0; i<5; i++){ // 對小數點後第一位做四捨五入
        cout << "結果" << i << ": " << fixed  <<  setprecision(0) << action[i] << endl;
    }
     
    return 0;
}