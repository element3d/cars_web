#ifndef __ESERIE__
#define __ESERIE__

#include <string>
#include <vector>
#include "EModel.h"

class ESerie
{
public:
   static std::string None;
   static std::string All;
   static std::string Bmw1;
   static std::string Bmw2;
   static std::string Bmw3;
   static std::string Bmw4;
   static std::string Bmw5;
   static std::string Bmw6;
   static std::string Bmw7;
   static std::string BmwX1;
   static std::string BmwX3;
   static std::string BmwX4;
   static std::string BmwX5;
   static std::string BmwX6;
   static std::string BmwX7;
   static std::string BmwZ4;
   static std::string BmwM;
   static std::string MercedesBenz190Class;
   static std::string MercedesBenzAClass;
   static std::string MercedesBenzBClass;
   static std::string MercedesBenzCClass;
   static std::string MercedesBenzEClass;
   static std::string MercedesBenzSClass;
   static std::string MercedesBenzCLClass;
   static std::string MercedesBenzCLAClass;
   static std::string MercedesBenzCLKClass;
   static std::string MercedesBenzCLSClass;
   static std::string MercedesBenzMLClass;
   static std::string MercedesBenzGClass;
   static std::string MercedesBenzGLClass;
   static std::string MercedesBenzGLAClass;
   static std::string MercedesBenzGLCCLass;
   static std::string MercedesBenzGLEClass;
   static std::string MercedesBenzGLKClass;
   static std::string MercedesBenzGLSClass;
   static std::string MercedesBenzRClass;
   static std::string MercedesBenzSLClass;
   static std::string MercedesBenzSLKClass;
   static std::string MercedesBenzVClass;
   static std::string MercedesBenzSprinter;
   static std::string MercedesBenzVaneo;
   static std::string MercedesBenzViano;
   static std::string MercedesBenzVito;
   static std::string MercedesBenzAMG;
   static std::string AudiA3;
   static std::string AudiA4;
   static std::string AudiA5;
   static std::string AudiA6;
   static std::string AudiA7;
   static std::string AudiA8;
   static std::string AudiQ5;
   static std::string AudiQ7;
   static std::string AudiTT;
   static std::string AudiRS;
   static std::string PorscheCayenne;
   static std::string PorscheMacan;
   static std::string PorschePanamera;
   static std::string LexusCT;
   static std::string LexusIS;
   static std::string LexusES;
   static std::string LexusGS;
   static std::string LexusLS;
   static std::string LexusNX;
   static std::string LexusRX;
   static std::string LexusGX;
   static std::string LexusLX;
   static std::string LexusF;
   static std::string InfinitiEXSeries;
   static std::string InfinitiJXSeries;
   static std::string InfinitiFXSeries;
   static std::string InfinityGSeries;
   static std::string InfinityMSeries;
   static std::string InfinitiQ3Series;
   static std::string InfinitiQX4Series;
   static std::string InfinitiQX50Series;
   static std::string InfinitiQX56Series;
   static std::string InfinitiQX60Series;
   static std::string InfinitiQX70Series;
   static std::string InfinitiQX80Series;
   static std::string ToyotaAlphard;
   static std::string ToyotaAltezza;
   static std::string ToyotaAvalon;
   static std::string ToyotaAvensis;
   static std::string ToyotaBelta;
   static std::string ToyotaBlade;
   static std::string ToyotaCHR;
   static std::string ToyotaCamry;
   static std::string ToyotaCorolla;
   static std::string ToyotaEstima;
   static std::string ToyotaFJCruiser;
   static std::string ToyotaFortuner;
   static std::string ToyotaHarrier;
   static std::string ToyotaHighlander;
   static std::string ToyotaHilux;
   static std::string ToyotaIpsum;
   static std::string ToyotaIst;
   static std::string ToyotaLandCruiser;
   static std::string ToyotaLandCruiserPrado;
   static std::string ToyotaNoah;
   static std::string ToyotaPasso;
   static std::string ToyotaPrius;
   static std::string ToyotaRAV4;
   static std::string ToyotaSequoia;
   static std::string ToyotaSienna;
   static std::string ToyotaTacoma;
   static std::string ToyotaTundra;
   static std::string ToyotaVellfire;
   static std::string ToyotaVitz;
   static std::string ToyotaVoxy;
   static std::string ToyotaYaris;
   static std::string NissanAlmera;
   static std::string NissanAltima;
   static std::string NissanArmada;
   static std::string NissanBluebird;
   static std::string NissanCube;
   static std::string NissanElgrand;
   static std::string NissanFrontier;
   static std::string NissanFuga;
   static std::string NissanJuke;
   static std::string NissanKicks;
   static std::string NissanLatio;
   static std::string NissanLeaf;
   static std::string NissanMarch;
   static std::string NissanMaxima;
   static std::string NissanMicra;
   static std::string NissanMurano;
   static std::string NissanNote;
   static std::string NissanPathfinder;
   static std::string NissanPatrol;
   static std::string NissanPresage;
   static std::string NissanQashqai;
   static std::string NissanQuest;
   static std::string NissanRogue;
   static std::string NissanSentra;
   static std::string NissanSerena;
   static std::string NissanSkyline;
   static std::string NissanSunny;
   static std::string NissanTeana;
   static std::string NissanTerrano;
   static std::string NissanTiida;
   static std::string NissanVersa;
   static std::string NissanXTrail;
   static std::string NissanXTerra;
   static std::string Nissan350Z;
   static std::string FordEdge;
   static std::string FordEscape;
   static std::string FordExplorer;
   static std::string FordF150;
   static std::string FordFiesta;
   static std::string FordFocus;
   static std::string FordFusion;
   static std::string FordTaurus;
   static std::string FordTransit;
   static std::string FordMustang;
   static std::string ChevroletAvalanche;
   static std::string ChevroletAveo;
   static std::string ChevroletCaptiva;
   static std::string ChevroletCruze;
   static std::string ChevroletEpica;
   static std::string ChevroletEquinox;
   static std::string ChevroletMalibu;
   static std::string ChevroletNiva;
   static std::string ChevroletTahoe;
   static std::string ChevroletTrailBlazer;
   static std::string ChevroletVolt;
   static std::string ChevroletCamaro;
   static std::string ChevroletCorvette;
   static std::string VolkswagenBeetle;
   static std::string VolkswagenBora;
   static std::string VolkswagenCaddy;
   static std::string VolkswagenCrafter;
   static std::string VolkswagenGolf;
   static std::string VolkswagenJetta;
   static std::string VolkswagenPassat;
   static std::string VolkswagenPassatCC;
   static std::string VolkswagenPolo;
   static std::string VolkswagenRoutan;
   static std::string VolkswagenTiguan;
   static std::string VolkswagenTouareg;
   static std::string VolkswagenTouran;
   static std::string VolkswagenTransporter;
   static std::string VolkswagenVento;
   static std::string OpelAstra;
   static std::string OpelCombo;
   static std::string OpelCorsa;
   static std::string OpelFrontera;
   static std::string OpelInsignia;
   static std::string OpelMeriva;
   static std::string OpelOmega;
   static std::string OpelSignum;
   static std::string OpelSintra;
   static std::string OpelVectra;
   static std::string OpelZafira;
   static std::string MitsubishiAirtrek;
   static std::string MitsubishiChariot;
   static std::string MitsubishiColt;
   static std::string MitsubishiGalant;
   static std::string MitsubishiGrandis;
   static std::string MitsubishiLancer;
   static std::string MitsubishiMontero;
   static std::string MitsubishiOutlander;
   static std::string MitsubishiPajero;
   static std::string KIACadenza;
   static std::string KIACerato;
   static std::string KIAForte;
   static std::string KIAK5;
   static std::string KIAOptima;
   static std::string KIARio;
   static std::string KIASedona;
   static std::string KIASorento;
   static std::string KIASportage;
   static std::string HyundaiAccent;
   static std::string HyundaiCreta;
   static std::string HyundaiElantra;
   static std::string HyundaiGenesis;
   static std::string HyundaiGenesisCoupe;
   static std::string HyundaiIX35;
   static std::string HyundaiKona;
   static std::string HyundaiSantaFe;
   static std::string HyundaiSolaris;
   static std::string HyundaiSonata;
   static std::string HyundaiTucson;
   static std::string HyundaiVeloster;
   static std::string HondaAccord;
   static std::string HondaCivic;
   static std::string HondaCRV;
   static std::string HondaCrosstour;
   static std::string HondaElyson;
   static std::string HondaFit;
   static std::string HondaHRV;
   static std::string HondaOdyssey;
   static std::string HondaPilot;
   static std::string HondaRidgeline;
   static std::string HondaStepwgn;
   static std::string MazdaAtenza;
   static std::string MazdaCX3;
   static std::string MazdaCX5;
   static std::string MazdaCX7;
   static std::string MazdaCX9;
   static std::string MazdaDemio;
   static std::string MazdaMPV;
   static std::string MazdaRX8;
   static std::string MazdaTribute;
   static std::string Mazda3;
   static std::string Mazda5;
   static std::string Mazda6;

   static std::string ToString(const std::string& serie) {
       if (ESerie::Bmw1 == serie) return "1 SERIES";
       if (ESerie::Bmw2 == serie) return "2 SERIES";
       if (ESerie::Bmw3 == serie) return "3 SERIES";
       if (ESerie::Bmw4 == serie) return "4 SERIES";
       if (ESerie::Bmw5 == serie) return "5 SERIES";
       if (ESerie::Bmw6 == serie) return "6 SERIES";
       if (ESerie::Bmw7 == serie) return "7 SERIES";
       if (ESerie::BmwX1 == serie) return "X1 SERIES";
       if (ESerie::BmwX3 == serie) return "X3 SERIES";
       if (ESerie::BmwX4 == serie) return "X4 SERIES";
       if (ESerie::BmwX5 == serie) return "X5 SERIES";
       if (ESerie::BmwX6 == serie) return "X6 SERIES";
       if (ESerie::BmwX7 == serie) return "X7 SERIES";
       if (ESerie::BmwZ4 == serie) return "Z4 SERIES";
       if (ESerie::BmwM == serie) return "M SERIES";
       if (ESerie::MercedesBenz190Class == serie) return "190 CLASS";
       if (ESerie::MercedesBenzAClass == serie) return "A CLASS";
       if (ESerie::MercedesBenzBClass == serie) return "B CLASS";
       if (ESerie::MercedesBenzCClass == serie) return "C CLASS";
       if (ESerie::MercedesBenzEClass == serie) return "E CLASS";
       if (ESerie::MercedesBenzSClass == serie) return "S CLASS";
       if (ESerie::MercedesBenzCLClass == serie) return "CL CLASS";
       if (ESerie::MercedesBenzCLAClass == serie) return "CLA CLASS";
       if (ESerie::MercedesBenzCLKClass == serie) return "CLK CLASS";
       if (ESerie::MercedesBenzCLSClass == serie) return "CLS CLASS";
       if (ESerie::MercedesBenzMLClass == serie) return "ML CLASS";
       if (ESerie::MercedesBenzGClass == serie) return "G CLASS";
       if (ESerie::MercedesBenzGLClass == serie) return "GL CLASS";
       if (ESerie::MercedesBenzGLAClass == serie) return "GLA CLASS";
       if (ESerie::MercedesBenzGLCCLass == serie) return "GLC CLASS";
       if (ESerie::MercedesBenzGLEClass == serie) return "GLE CLASS";
       if (ESerie::MercedesBenzGLKClass == serie) return "GLK CLASS";
       if (ESerie::MercedesBenzGLSClass == serie) return "GLS CLASS";
       if (ESerie::MercedesBenzRClass == serie) return "R CLASS";
       if (ESerie::MercedesBenzSLClass == serie) return "SL CLASS";
       if (ESerie::MercedesBenzSLKClass == serie) return "SLK CLASS";
       if (ESerie::MercedesBenzVClass == serie) return "V CLASS";
       if (ESerie::MercedesBenzSprinter == serie) return "SPRINTER";
       if (ESerie::MercedesBenzVaneo == serie) return "VANEO";
       if (ESerie::MercedesBenzViano == serie) return "VIANO";
       if (ESerie::MercedesBenzVito == serie) return "VITO";
       if (ESerie::MercedesBenzAMG == serie) return "AMG";
       if (ESerie::AudiA3 == serie) return "A3 CLASS";
       if (ESerie::AudiA4 == serie) return "A4 CLASS";
       if (ESerie::AudiA5 == serie) return "A5 CLASS";
       if (ESerie::AudiA6 == serie) return "A6 CLASS";
       if (ESerie::AudiA7 == serie) return "A7 CLASS";
       if (ESerie::AudiA8 == serie) return "A8 CLASS";
       if (ESerie::AudiQ5 == serie) return "Q5 CLASS";
       if (ESerie::AudiQ7 == serie) return "Q7 CLASS";
       if (ESerie::AudiTT == serie) return "TT CLASS";
       if (ESerie::AudiRS == serie) return "RS CLASS";
       if (ESerie::PorscheCayenne == serie) return "CAYENNE";
       if (ESerie::PorscheMacan == serie) return "MACAN";
       if (ESerie::PorschePanamera == serie) return "PANAMERA";
       if (ESerie::LexusCT == serie) return "CT CLASS";
       if (ESerie::LexusIS == serie) return "IS CLASS";
       if (ESerie::LexusES == serie) return "ES CLASS";
       if (ESerie::LexusGS == serie) return "GS CLASS";
       if (ESerie::LexusLS == serie) return "LS CLASS";
       if (ESerie::LexusNX == serie) return "NX CLASS";
       if (ESerie::LexusRX == serie) return "RX CLASS";
       if (ESerie::LexusGX == serie) return "GX CLASS";
       if (ESerie::LexusLX == serie) return "LX CLASS";
       if (ESerie::LexusF == serie) return "F CLASS";
       if (ESerie::InfinitiEXSeries == serie) return "EX SERIES";
       if (ESerie::InfinitiJXSeries == serie) return "JX SERIES";
       if (ESerie::InfinitiFXSeries == serie) return "FX SERIES";
       if (ESerie::InfinityGSeries == serie) return "G SERIES";
       if (ESerie::InfinityMSeries == serie) return "M SERIES";
       if (ESerie::InfinitiQ3Series == serie) return "Q3 SERIES";
       if (ESerie::InfinitiQX4Series == serie) return "QX4 SERIES";
       if (ESerie::InfinitiQX50Series == serie) return "QX50 SERIES";
       if (ESerie::InfinitiQX56Series == serie) return "QX56 SERIES";
       if (ESerie::InfinitiQX60Series == serie) return "QX60 SERIES";
       if (ESerie::InfinitiQX70Series == serie) return "QX70 SERIES";
       if (ESerie::InfinitiQX80Series == serie) return "QX80 SERIES";
       if (ESerie::ToyotaAlphard == serie) return "ALPHARD";
       if (ESerie::ToyotaAltezza == serie) return "ALTEZZA";
       if (ESerie::ToyotaAvalon == serie) return "AVALON";
       if (ESerie::ToyotaAvensis == serie) return "AVENSIS";
       if (ESerie::ToyotaBelta == serie) return "BELTA";
       if (ESerie::ToyotaBlade == serie) return "BLADE";
       if (ESerie::ToyotaCHR == serie) return "CHR";
       if (ESerie::ToyotaCamry == serie) return "CAMRY";
       if (ESerie::ToyotaCorolla == serie) return "COROLLA";
       if (ESerie::ToyotaEstima == serie) return "ESTIMA";
       if (ESerie::ToyotaFJCruiser == serie) return "FJ CRUISER";
       if (ESerie::ToyotaFortuner == serie) return "FORTUNER";
       if (ESerie::ToyotaHarrier == serie) return "HARRIER";
       if (ESerie::ToyotaHighlander == serie) return "HIGHLANDER";
       if (ESerie::ToyotaHilux == serie) return "HILUX";
       if (ESerie::ToyotaIpsum == serie) return "IPSUM";
       if (ESerie::ToyotaIst == serie) return "IST";
       if (ESerie::ToyotaLandCruiser == serie) return "LAND CRUISER";
       if (ESerie::ToyotaLandCruiserPrado == serie) return "LAND CRUISER PRADO";
       if (ESerie::ToyotaNoah == serie) return "NOAH";
       if (ESerie::ToyotaPasso == serie) return "PASSO";
       if (ESerie::ToyotaPrius == serie) return "PRIUS";
       if (ESerie::ToyotaRAV4 == serie) return "RAV4";
       if (ESerie::ToyotaSequoia == serie) return "SEQUOIA";
       if (ESerie::ToyotaSienna == serie) return "SIENNA";
       if (ESerie::ToyotaTacoma == serie) return "TACOMA";
       if (ESerie::ToyotaTundra == serie) return "TUNDRA";
       if (ESerie::ToyotaVellfire == serie) return "VELLFIRE";
       if (ESerie::ToyotaVitz == serie) return "VITZ";
       if (ESerie::ToyotaVoxy == serie) return "VOXY";
       if (ESerie::ToyotaYaris == serie) return "YARIS";
       if (ESerie::NissanAlmera == serie) return "ALMERA";
       if (ESerie::NissanAltima == serie) return "ALTIMA";
       if (ESerie::NissanArmada == serie) return "ARMADA";
       if (ESerie::NissanBluebird == serie) return "BLUEBIRD";
       if (ESerie::NissanCube == serie) return "CUBE";
       if (ESerie::NissanElgrand == serie) return "ELGRAND";
       if (ESerie::NissanFrontier == serie) return "FRONTIER";
       if (ESerie::NissanFuga == serie) return "FUGA";
       if (ESerie::NissanJuke == serie) return "JUKE";
       if (ESerie::NissanKicks == serie) return "KICKS";
       if (ESerie::NissanLatio == serie) return "LATIO";
       if (ESerie::NissanLeaf == serie) return "LEAF";
       if (ESerie::NissanMarch == serie) return "MARCH";
       if (ESerie::NissanMaxima == serie) return "MAXIMA";
       if (ESerie::NissanMicra == serie) return "MICRA";
       if (ESerie::NissanMurano == serie) return "MURANO";
       if (ESerie::NissanNote == serie) return "NOTE";
       if (ESerie::NissanPathfinder == serie) return "PATHFINDER";
       if (ESerie::NissanPatrol == serie) return "PATROL";
       if (ESerie::NissanPresage == serie) return "PRESAGE";
       if (ESerie::NissanQashqai == serie) return "QASHQAI";
       if (ESerie::NissanQuest == serie) return "QUEST";
       if (ESerie::NissanRogue == serie) return "ROGUE";
       if (ESerie::NissanSentra == serie) return "SENTRA";
       if (ESerie::NissanSerena == serie) return "SERENA";
       if (ESerie::NissanSkyline == serie) return "SKYLINE";
       if (ESerie::NissanSunny == serie) return "SUNNY";
       if (ESerie::NissanTeana == serie) return "TEANA";
       if (ESerie::NissanTerrano == serie) return "TERRANO";
       if (ESerie::NissanTiida == serie) return "TIIDA";
       if (ESerie::NissanVersa == serie) return "VERSA";
       if (ESerie::NissanXTrail == serie) return "X-TRAIL";
       if (ESerie::NissanXTerra == serie) return "X-TERRA";
       if (ESerie::Nissan350Z == serie) return "350Z";
       if (ESerie::FordEdge == serie) return "EDGE";
       if (ESerie::FordEscape == serie) return "ESCAPE";
       if (ESerie::FordExplorer == serie) return "EXPLORER";
       if (ESerie::FordF150 == serie) return "F150";
       if (ESerie::FordFiesta == serie) return "FIESTA";
       if (ESerie::FordFocus == serie) return "FOCUS";
       if (ESerie::FordFusion == serie) return "FUSION";
       if (ESerie::FordTaurus == serie) return "TAURUS";
       if (ESerie::FordTransit == serie) return "TRANSIT";
       if (ESerie::FordMustang == serie) return "MUSTANG";
       if (ESerie::ChevroletAvalanche == serie) return "AVALANCHE";
       if (ESerie::ChevroletAveo == serie) return "AVEO";
       if (ESerie::ChevroletCaptiva == serie) return "CAPTIVA";
       if (ESerie::ChevroletCruze == serie) return "CRUZE";
       if (ESerie::ChevroletEpica == serie) return "EPICA";
       if (ESerie::ChevroletEquinox == serie) return "EQUINOX";
       if (ESerie::ChevroletMalibu == serie) return "MALIBU";
       if (ESerie::ChevroletNiva == serie) return "NIVA";
       if (ESerie::ChevroletTahoe == serie) return "TAHOE";
       if (ESerie::ChevroletTrailBlazer == serie) return "TRAIL BLAZER";
       if (ESerie::ChevroletVolt == serie) return "VOLT";
       if (ESerie::ChevroletCamaro == serie) return "CAMARO";
       if (ESerie::ChevroletCorvette == serie) return "CORVETTE";
       if (ESerie::VolkswagenBeetle == serie) return "BEETLE";
       if (ESerie::VolkswagenBora == serie) return "BORA";
       if (ESerie::VolkswagenCaddy == serie) return "CADDY";
       if (ESerie::VolkswagenCrafter == serie) return "CRAFTER";
       if (ESerie::VolkswagenGolf == serie) return "GOLF";
       if (ESerie::VolkswagenJetta == serie) return "JETTA";
       if (ESerie::VolkswagenPassat == serie) return "PASSAT";
       if (ESerie::VolkswagenPassatCC == serie) return "PASSAT CC";
       if (ESerie::VolkswagenPolo == serie) return "POLO";
       if (ESerie::VolkswagenRoutan == serie) return "ROUTAN";
       if (ESerie::VolkswagenTiguan == serie) return "TIGUAN";
       if (ESerie::VolkswagenTouareg == serie) return "TOUAREG";
       if (ESerie::VolkswagenTouran == serie) return "TOURAN";
       if (ESerie::VolkswagenTransporter == serie) return "TRANSPORTER";
       if (ESerie::VolkswagenVento == serie) return "VENTO";
       if (ESerie::OpelAstra == serie) return "ASTRA";
       if (ESerie::OpelCombo == serie) return "COMBO";
       if (ESerie::OpelCorsa == serie) return "CORSÁ";
       if (ESerie::OpelFrontera == serie) return "FRONTERA";
       if (ESerie::OpelInsignia == serie) return "INSIGNIA";
       if (ESerie::OpelMeriva == serie) return "MERIVA";
       if (ESerie::OpelOmega == serie) return "OMEGA";
       if (ESerie::OpelSignum == serie) return "SIGNUM";
       if (ESerie::OpelSintra == serie) return "SINTRA";
       if (ESerie::OpelVectra == serie) return "VECTRA";
       if (ESerie::OpelZafira == serie) return "ZAFIRA";
       if (ESerie::MitsubishiAirtrek == serie) return "AIRTREK";
       if (ESerie::MitsubishiChariot == serie) return "CHARIOT";
       if (ESerie::MitsubishiColt == serie) return "COLT";
       if (ESerie::MitsubishiGalant == serie) return "GALANT";
       if (ESerie::MitsubishiGrandis == serie) return "GRANDIS";
       if (ESerie::MitsubishiLancer == serie) return "LANCER";
       if (ESerie::MitsubishiMontero == serie) return "MONTERO";
       if (ESerie::MitsubishiOutlander == serie) return "OUTLANDER";
       if (ESerie::MitsubishiPajero == serie) return "PAJERO";
       if (ESerie::KIACadenza == serie) return "CADENZA";
       if (ESerie::KIACerato == serie) return "CERATO";
       if (ESerie::KIAForte == serie) return "FORTE";
       if (ESerie::KIAK5 == serie) return "K5";
       if (ESerie::KIAOptima == serie) return "OPTIMA";
       if (ESerie::KIARio == serie) return "RIO";
       if (ESerie::KIASedona == serie) return "SEDONA";
       if (ESerie::KIASorento == serie) return "SORENTO";
       if (ESerie::KIASportage == serie) return "SPORTAGE";
       if (ESerie::HyundaiAccent == serie) return "ACCENT";
       if (ESerie::HyundaiCreta == serie) return "CRETA";
       if (ESerie::HyundaiElantra == serie) return "ELANTRA";
       if (ESerie::HyundaiGenesis == serie) return "GENESIS";
       if (ESerie::HyundaiGenesisCoupe == serie) return "GENESIS COUPE";
       if (ESerie::HyundaiIX35 == serie) return "IX35";
       if (ESerie::HyundaiKona == serie) return "KONA";
       if (ESerie::HyundaiSantaFe == serie) return "SANTA FE";
       if (ESerie::HyundaiSolaris == serie) return "SOLARIS";
       if (ESerie::HyundaiSonata == serie) return "SONATA";
       if (ESerie::HyundaiTucson == serie) return "TUCSON";
       if (ESerie::HyundaiVeloster == serie) return "VELOSTER";
       if (ESerie::HondaAccord == serie) return "ACCORD";
       if (ESerie::HondaCivic == serie) return "CIVIC";
       if (ESerie::HondaCRV == serie) return "CRV";
       if (ESerie::HondaCrosstour == serie) return "CROSSTOUR";
       if (ESerie::HondaElyson == serie) return "ELYSON";
       if (ESerie::HondaFit == serie) return "FIT";
       if (ESerie::HondaHRV == serie) return "HRV";
       if (ESerie::HondaOdyssey == serie) return "ODYSSEY";
       if (ESerie::HondaPilot == serie) return "PILOT";
       if (ESerie::HondaRidgeline == serie) return "RIDGELINE";
       if (ESerie::HondaStepwgn == serie) return "STEPWGN";
       if (ESerie::MazdaAtenza == serie) return "ATENZA";
       if (ESerie::MazdaCX3 == serie) return "CX3";
       if (ESerie::MazdaCX5 == serie) return "CX5";
       if (ESerie::MazdaCX7 == serie) return "CX7";
       if (ESerie::MazdaCX9 == serie) return "CX9";
       if (ESerie::MazdaDemio == serie) return "DEMIO";
       if (ESerie::MazdaMPV == serie) return "MPV";
       if (ESerie::MazdaRX8 == serie) return "RX8";
       if (ESerie::MazdaTribute == serie) return "TRIBUTE";
       if (ESerie::Mazda3 == serie) return "3 CLASS";
       if (ESerie::Mazda5 == serie) return "5 CLASS";
       if (ESerie::Mazda6 == serie) return "6 CLASS";
       return "";
   }
    static std::vector<std::string> ModelsToArray(const std::string& serie)
    {
        std::vector<std::string> vec;
        if (ESerie::Bmw1 == serie)
        {
            vec.push_back(EModel::Bmw1E81);
            vec.push_back(EModel::Bmw1E82);
            vec.push_back(EModel::Bmw1E87);
            vec.push_back(EModel::Bmw1E88);
            vec.push_back(EModel::Bmw1F20);
            vec.push_back(EModel::Bmw1F21);
            vec.push_back(EModel::Bmw1F40);
            vec.push_back(EModel::Bmw1F52);
        }
        if (ESerie::Bmw2 == serie)
        {
            vec.push_back(EModel::Bmw2F22);
            vec.push_back(EModel::Bmw2F23);
            vec.push_back(EModel::Bmw2F45);
            vec.push_back(EModel::Bmw2F46);
            vec.push_back(EModel::Bmw2F44);
            vec.push_back(EModel::Bmw2G42);
        }
        if (ESerie::Bmw3 == serie)
        {
            vec.push_back(EModel::Bmw3E30);
            vec.push_back(EModel::Bmw3E36);
            vec.push_back(EModel::Bmw3E46);
            vec.push_back(EModel::Bmw3E90);
            vec.push_back(EModel::Bmw3E92);
            vec.push_back(EModel::Bmw3F30);
            vec.push_back(EModel::Bmw3G20);
        }
        if (ESerie::Bmw4 == serie)
        {
            vec.push_back(EModel::Bmw4F32);
            vec.push_back(EModel::Bmw4F33);
            vec.push_back(EModel::Bmw4F36);
            vec.push_back(EModel::Bmw4G22);
            vec.push_back(EModel::Bmw4G23);
            vec.push_back(EModel::Bmw4G26);
        }
        if (ESerie::Bmw5 == serie)
        {
            vec.push_back(EModel::Bmw5E34);
            vec.push_back(EModel::Bmw5E39);
            vec.push_back(EModel::Bmw5E60);
            vec.push_back(EModel::Bmw5E61);
            vec.push_back(EModel::Bmw5F10);
            vec.push_back(EModel::Bmw5F11);
            vec.push_back(EModel::Bmw5F07);
            vec.push_back(EModel::Bmw5F18);
            vec.push_back(EModel::Bmw5G30);
            vec.push_back(EModel::Bmw5G31);
            vec.push_back(EModel::Bmw5G38);
        }
        if (ESerie::Bmw6 == serie)
        {
            vec.push_back(EModel::Bmw6E63);
            vec.push_back(EModel::Bmw6E64);
            vec.push_back(EModel::Bmw6F06);
            vec.push_back(EModel::Bmw6F12);
            vec.push_back(EModel::Bmw6F13);
            vec.push_back(EModel::Bmw6G32);
        }
        if (ESerie::Bmw7 == serie)
        {
            vec.push_back(EModel::Bmw7E38);
            vec.push_back(EModel::Bmw7E65);
            vec.push_back(EModel::Bmw7E66);
            vec.push_back(EModel::Bmw7E67);
            vec.push_back(EModel::Bmw7F68);
            vec.push_back(EModel::Bmw7F01);
            vec.push_back(EModel::Bmw7F02);
            vec.push_back(EModel::Bmw7F03);
            vec.push_back(EModel::Bmw7F04);
            vec.push_back(EModel::Bmw7G11);
            vec.push_back(EModel::Bmw7G12);
        }
        if (ESerie::BmwX1 == serie)
        {
            vec.push_back(EModel::BmwX1E84);
            vec.push_back(EModel::BmwX1F48);
        }
        if (ESerie::BmwX3 == serie)
        {
            vec.push_back(EModel::BmwX3E83);
            vec.push_back(EModel::BmwX3F25);
            vec.push_back(EModel::BmwX3G01);
        }
        if (ESerie::BmwX4 == serie)
        {
            vec.push_back(EModel::BmwX4F26);
            vec.push_back(EModel::BmwX4G02);
        }
        if (ESerie::BmwX5 == serie)
        {
            vec.push_back(EModel::BmwX5E53);
            vec.push_back(EModel::BmwX5E70);
            vec.push_back(EModel::BmwX5F15);
            vec.push_back(EModel::BmwX5G05);
        }
        if (ESerie::BmwX6 == serie)
        {
            vec.push_back(EModel::BmwX6E71);
            vec.push_back(EModel::BmwX6F16);
            vec.push_back(EModel::BmwX6G06);
        }
        if (ESerie::BmwX7 == serie)
        {
            vec.push_back(EModel::BmwX7G07);
        }
        if (ESerie::BmwZ4 == serie)
        {
            vec.push_back(EModel::BmwZ4E85);
            vec.push_back(EModel::BmwZ4E86);
            vec.push_back(EModel::BmwZ4E89);
            vec.push_back(EModel::BmwZ4G29);
        }
        if (ESerie::BmwM == serie)
        {
        }
        if (ESerie::MercedesBenz190Class == serie)
        {
            vec.push_back(EModel::MercedesBenz190W201);
        }
        if (ESerie::MercedesBenzAClass == serie)
        {
            vec.push_back(EModel::MercedesBenzAW168);
            vec.push_back(EModel::MercedesBenzAW169);
            vec.push_back(EModel::MercedesBenzAW176);
            vec.push_back(EModel::MercedesBenzAW177);
        }
        if (ESerie::MercedesBenzBClass == serie)
        {
            vec.push_back(EModel::MercedesBenzBW245);
            vec.push_back(EModel::MercedesBenzBW246);
            vec.push_back(EModel::MercedesBenzBW247);
        }
        if (ESerie::MercedesBenzCClass == serie)
        {
            vec.push_back(EModel::MercedesBencCW202);
            vec.push_back(EModel::MercedesBencCW203);
            vec.push_back(EModel::MercedesBencCW204);
            vec.push_back(EModel::MercedesBencCW205);
            vec.push_back(EModel::MercedesBencCW206);
        }
        if (ESerie::MercedesBenzEClass == serie)
        {
            vec.push_back(EModel::MercedesBencEW124);
            vec.push_back(EModel::MercedesBencEW210);
            vec.push_back(EModel::MercedesBencEW211);
            vec.push_back(EModel::MercedesBencEW212);
            vec.push_back(EModel::MercedesBencEC207);
            vec.push_back(EModel::MercedesBencEA207);
            vec.push_back(EModel::MercedesBencEW213);
        }
        if (ESerie::MercedesBenzSClass == serie)
        {
            vec.push_back(EModel::MercedesBencSW140);
            vec.push_back(EModel::MercedesBencSW220);
            vec.push_back(EModel::MercedesBencSW221);
            vec.push_back(EModel::MercedesBencSW222);
            vec.push_back(EModel::MercedesBencSW223);
        }
        if (ESerie::MercedesBenzCLClass == serie)
        {
            vec.push_back(EModel::MercedesBencCLC140);
            vec.push_back(EModel::MercedesBencCLC215);
            vec.push_back(EModel::MercedesBencCLC216);
        }
        if (ESerie::MercedesBenzCLAClass == serie)
        {
            vec.push_back(EModel::MercedesBencCLAC117);
        }
        if (ESerie::MercedesBenzCLKClass == serie)
        {
            vec.push_back(EModel::MercedesBencCLKC208);
            vec.push_back(EModel::MercedesBencCLKA208);
            vec.push_back(EModel::MercedesBencCLKC209);
            vec.push_back(EModel::MercedesBencCLKA209);
        }
        if (ESerie::MercedesBenzCLSClass == serie)
        {
            vec.push_back(EModel::MercedesBencCLSC219);
            vec.push_back(EModel::MercedesBencCLSC256);
        }
        if (ESerie::MercedesBenzMLClass == serie)
        {
            vec.push_back(EModel::MercedesBencMLW163);
            vec.push_back(EModel::MercedesBencMLW164);
            vec.push_back(EModel::MercedesBencMLW166);
            vec.push_back(EModel::MercedesBencMLW167);
        }
        if (ESerie::MercedesBenzGClass == serie)
        {
            vec.push_back(EModel::MercedesBencGW463);
        }
        if (ESerie::MercedesBenzGLClass == serie)
        {
            vec.push_back(EModel::MercedesBencGLX164);
            vec.push_back(EModel::MercedesBencGLX166);
        }
        if (ESerie::MercedesBenzGLAClass == serie)
        {
            vec.push_back(EModel::MercedesBencGLAX156);
            vec.push_back(EModel::MercedesBencGLAH247);
        }
        if (ESerie::MercedesBenzGLCCLass == serie)
        {
            vec.push_back(EModel::MercedesBencGLCX253);
            vec.push_back(EModel::MercedesBencGLCC253);
        }
        if (ESerie::MercedesBenzGLEClass == serie)
        {
            vec.push_back(EModel::MercedesBencGLEV167);
        }
        if (ESerie::MercedesBenzGLKClass == serie)
        {
            vec.push_back(EModel::MercedesBencGLKX204);
        }
        if (ESerie::MercedesBenzGLSClass == serie)
        {
            vec.push_back(EModel::MercedesBencGLSX167);
        }
        if (ESerie::MercedesBenzRClass == serie)
        {
            vec.push_back(EModel::MercedesBencRW251);
        }
        if (ESerie::MercedesBenzSLClass == serie)
        {
            vec.push_back(EModel::MercedesBencSLR230);
            vec.push_back(EModel::MercedesBencSLR231);
        }
        if (ESerie::MercedesBenzSLKClass == serie)
        {
            vec.push_back(EModel::MercedesBencSLKR170);
            vec.push_back(EModel::MercedesBencSLKR171);
            vec.push_back(EModel::MercedesBencSLKR172);
        }
        if (ESerie::MercedesBenzVClass == serie)
        {
            vec.push_back(EModel::MercedesBencVW638);
            vec.push_back(EModel::MercedesBencVW639);
            vec.push_back(EModel::MercedesBencVW447);
        }
        if (ESerie::MercedesBenzSprinter == serie)
        {
            vec.push_back(EModel::MercedesBencSprinterW901);
            vec.push_back(EModel::MercedesBencSprinterW905);
            vec.push_back(EModel::MercedesBencSprinternNCV3);
        }
        if (ESerie::MercedesBenzVaneo == serie)
        {
            vec.push_back(EModel::MercedesBencVaneoW414);
        }
        if (ESerie::MercedesBenzViano == serie)
        {
            vec.push_back(EModel::MercedesBencViano);
        }
        if (ESerie::MercedesBenzVito == serie)
        {
            vec.push_back(EModel::MercedesBencVito);
        }
        if (ESerie::MercedesBenzAMG == serie)
        {
        }
        if (ESerie::AudiA3 == serie)
        {
            vec.push_back(EModel::AudiA38L);
            vec.push_back(EModel::AudiA38P);
            vec.push_back(EModel::AudiA38V);
            vec.push_back(EModel::AudiA38Y);
        }
        if (ESerie::AudiA4 == serie)
        {
            vec.push_back(EModel::AudiA4B5);
            vec.push_back(EModel::AudiA4B6);
            vec.push_back(EModel::AudiA4B7);
            vec.push_back(EModel::AudiA4B8);
            vec.push_back(EModel::AudiA4B9);
        }
        if (ESerie::AudiA5 == serie)
        {
            vec.push_back(EModel::AudiA58T);
            vec.push_back(EModel::AudiA5F5);
        }
        if (ESerie::AudiA6 == serie)
        {
            vec.push_back(EModel::AudiA6C5);
            vec.push_back(EModel::AudiA6C6);
            vec.push_back(EModel::AudiA6C7);
            vec.push_back(EModel::AudiA6C8);
        }
        if (ESerie::AudiA7 == serie)
        {
            vec.push_back(EModel::AudiA7G4);
            vec.push_back(EModel::AudiA74K);
        }
        if (ESerie::AudiA8 == serie)
        {
            vec.push_back(EModel::AudiA8D2);
            vec.push_back(EModel::AudiA8D3);
            vec.push_back(EModel::AudiA8D4);
            vec.push_back(EModel::AudiA8D5);
        }
        if (ESerie::AudiQ5 == serie)
        {
            vec.push_back(EModel::AudiQ58R);
            vec.push_back(EModel::AudiQ5FY);
        }
        if (ESerie::AudiQ7 == serie)
        {
            vec.push_back(EModel::AudiQ74L);
            vec.push_back(EModel::AudiQ74M);
        }
        if (ESerie::AudiTT == serie)
        {
            vec.push_back(EModel::AudiTT8N);
            vec.push_back(EModel::AudiTT8J);
            vec.push_back(EModel::AudiTT8S);
        }
        if (ESerie::AudiRS == serie)
        {
        }
        if (ESerie::PorscheCayenne == serie)
        {
            vec.push_back(EModel::PorscheCayenne1);
            vec.push_back(EModel::PorscheCayenne2);
            vec.push_back(EModel::PorscheCayenne3);
        }
        if (ESerie::PorscheMacan == serie)
        {
            vec.push_back(EModel::PorscheMacan1);
        }
        if (ESerie::PorschePanamera == serie)
        {
            vec.push_back(EModel::PorschePanamera1);
            vec.push_back(EModel::PorschePanamera2);
        }
        if (ESerie::LexusCT == serie)
        {
            vec.push_back(EModel::LexusCT1);
        }
        if (ESerie::LexusIS == serie)
        {
            vec.push_back(EModel::LexusIS1);
            vec.push_back(EModel::LexusIS2);
            vec.push_back(EModel::LexusIS3);
        }
        if (ESerie::LexusES == serie)
        {
            vec.push_back(EModel::LexusES4);
            vec.push_back(EModel::LexusES5);
            vec.push_back(EModel::LexusES6);
            vec.push_back(EModel::LexusES7);
        }
        if (ESerie::LexusGS == serie)
        {
            vec.push_back(EModel::LexusGS2);
            vec.push_back(EModel::LexusGS3);
            vec.push_back(EModel::LexusGS4);
        }
        if (ESerie::LexusLS == serie)
        {
            vec.push_back(EModel::LexusLS4);
            vec.push_back(EModel::LexusLS5);
        }
        if (ESerie::LexusNX == serie)
        {
            vec.push_back(EModel::LexusNX1);
            vec.push_back(EModel::LexusNX2);
        }
        if (ESerie::LexusRX == serie)
        {
            vec.push_back(EModel::LexusRX1);
            vec.push_back(EModel::LexusRX2);
            vec.push_back(EModel::LexusRX3);
            vec.push_back(EModel::LexusRX4);
        }
        if (ESerie::LexusGX == serie)
        {
            vec.push_back(EModel::LexusGX1);
            vec.push_back(EModel::LexusGX2);
        }
        if (ESerie::LexusLX == serie)
        {
            vec.push_back(EModel::LexusLX2);
            vec.push_back(EModel::LexusLX3);
        }
        if (ESerie::LexusF == serie)
        {
        }
        if (ESerie::InfinitiEXSeries == serie)
        {
            vec.push_back(EModel::InfinitiEX1J50);
        }
        if (ESerie::InfinitiJXSeries == serie)
        {
            vec.push_back(EModel::InfinitiJX1);
        }
        if (ESerie::InfinitiFXSeries == serie)
        {
            vec.push_back(EModel::InfinitiFX1S50);
            vec.push_back(EModel::InfinitiFX2S51);
        }
        if (ESerie::InfinityGSeries == serie)
        {
            vec.push_back(EModel::InfinitiG3);
            vec.push_back(EModel::InfinitiG4);
        }
        if (ESerie::InfinityMSeries == serie)
        {
            vec.push_back(EModel::InfinitiM3);
            vec.push_back(EModel::InfinitiM4);
        }
        if (ESerie::InfinitiQ3Series == serie)
        {
            vec.push_back(EModel::InfinitiQ3);
        }
        if (ESerie::InfinitiQX4Series == serie)
        {
            vec.push_back(EModel::InfinitiQX4);
        }
        if (ESerie::InfinitiQX50Series == serie)
        {
            vec.push_back(EModel::InfinitiQX501);
            vec.push_back(EModel::InfinitiQX502);
        }
        if (ESerie::InfinitiQX56Series == serie)
        {
            vec.push_back(EModel::InfinitiQX561);
            vec.push_back(EModel::InfinitiQX562);
        }
        if (ESerie::InfinitiQX60Series == serie)
        {
            vec.push_back(EModel::InfinitiQX601);
        }
        if (ESerie::InfinitiQX70Series == serie)
        {
            vec.push_back(EModel::InfinitiQX70);
        }
        if (ESerie::InfinitiQX80Series == serie)
        {
            vec.push_back(EModel::InfinitiQX801);
            vec.push_back(EModel::InfinitiQX802);
        }
        if (ESerie::ToyotaAlphard == serie)
        {
            vec.push_back(EModel::ToyotaAlphard1);
            vec.push_back(EModel::ToyotaAlphard2);
            vec.push_back(EModel::ToyotaAlphard3);
        }
        if (ESerie::ToyotaAltezza == serie)
        {
            vec.push_back(EModel::ToyotaAltezza);
        }
        if (ESerie::ToyotaAvalon == serie)
        {
            vec.push_back(EModel::ToyotaAvalon3);
            vec.push_back(EModel::ToyotaAvalon4);
            vec.push_back(EModel::ToyotaAvalon5);
        }
        if (ESerie::ToyotaAvensis == serie)
        {
            vec.push_back(EModel::ToyotaAvensis2);
            vec.push_back(EModel::ToyotaAvensis3);
        }
        if (ESerie::ToyotaBelta == serie)
        {
            vec.push_back(EModel::ToyotaBelta);
        }
        if (ESerie::ToyotaBlade == serie)
        {
            vec.push_back(EModel::ToyotaBlade1E150);
        }
        if (ESerie::ToyotaCHR == serie)
        {
            vec.push_back(EModel::ToyotaCHR1);
        }
        if (ESerie::ToyotaCamry == serie)
        {
            vec.push_back(EModel::ToyotaCamryXV40);
            vec.push_back(EModel::ToyotaCamryXV50);
            vec.push_back(EModel::ToyotaCamryXV70);
        }
        if (ESerie::ToyotaCorolla == serie)
        {
            vec.push_back(EModel::ToyotaCorollaE140);
            vec.push_back(EModel::ToyotaCorollaE160);
            vec.push_back(EModel::ToyotaCorollaE210);
        }
        if (ESerie::ToyotaEstima == serie)
        {
            vec.push_back(EModel::ToyotaEstima2);
            vec.push_back(EModel::ToyotaEstima3);
        }
        if (ESerie::ToyotaFJCruiser == serie)
        {
            vec.push_back(EModel::ToyotaFJCruiser);
        }
        if (ESerie::ToyotaFortuner == serie)
        {
            vec.push_back(EModel::ToyotaFortuner1);
            vec.push_back(EModel::ToyotaFortuner2);
        }
        if (ESerie::ToyotaHarrier == serie)
        {
            vec.push_back(EModel::ToyotaHarrierXU10);
            vec.push_back(EModel::ToyotaHarrierXU30);
            vec.push_back(EModel::ToyotaHarrierXU60);
        }
        if (ESerie::ToyotaHighlander == serie)
        {
            vec.push_back(EModel::ToyotaHighlanderU20);
            vec.push_back(EModel::ToyotaHighlanderU40);
            vec.push_back(EModel::ToyotaHighlanderU50);
            vec.push_back(EModel::ToyotaHighlanderU70);
        }
        if (ESerie::ToyotaHilux == serie)
        {
            vec.push_back(EModel::ToyotaHilux6);
            vec.push_back(EModel::ToyotaHilux7);
            vec.push_back(EModel::ToyotaHilux8);
        }
        if (ESerie::ToyotaIpsum == serie)
        {
            vec.push_back(EModel::ToyotaIpsumM10);
            vec.push_back(EModel::ToyotaIpsumM20);
        }
        if (ESerie::ToyotaIst == serie)
        {
            vec.push_back(EModel::ToyotaIst1);
            vec.push_back(EModel::ToyotaIst2);
        }
        if (ESerie::ToyotaLandCruiser == serie)
        {
            vec.push_back(EModel::ToyotaLandCruiser100);
            vec.push_back(EModel::ToyotaLandCruiser200);
            vec.push_back(EModel::ToyotaLandCruiser300);
        }
        if (ESerie::ToyotaLandCruiserPrado == serie)
        {
            vec.push_back(EModel::ToyotaLandCruiserPrado120);
            vec.push_back(EModel::ToyotaLandCruiserPrado150);
        }
        if (ESerie::ToyotaNoah == serie)
        {
            vec.push_back(EModel::ToyotaNoahR60);
            vec.push_back(EModel::ToyotaNoahR70);
            vec.push_back(EModel::ToyotaNoahR80);
        }
        if (ESerie::ToyotaPasso == serie)
        {
            vec.push_back(EModel::ToyotaPasso1);
            vec.push_back(EModel::ToyotaPasso2);
            vec.push_back(EModel::ToyotaPasso3);
        }
        if (ESerie::ToyotaPrius == serie)
        {
            vec.push_back(EModel::ToyotaPriusXW10);
            vec.push_back(EModel::ToyotaPriusXW20);
            vec.push_back(EModel::ToyotaPriusXW30);
            vec.push_back(EModel::ToyotaPriusXW50);
        }
        if (ESerie::ToyotaRAV4 == serie)
        {
            vec.push_back(EModel::ToyotaRAV4XA10);
            vec.push_back(EModel::ToyotaRAV4XA20);
            vec.push_back(EModel::ToyotaRAV4XA30);
            vec.push_back(EModel::ToyotaRAV4CA40);
            vec.push_back(EModel::ToyotaRAV4XA50);
        }
        if (ESerie::ToyotaSequoia == serie)
        {
            vec.push_back(EModel::ToyotaSequoia1);
            vec.push_back(EModel::ToyotaSequoia2);
        }
        if (ESerie::ToyotaSienna == serie)
        {
            vec.push_back(EModel::ToyotaSienna1);
            vec.push_back(EModel::ToyotaSienna2);
            vec.push_back(EModel::ToyotaSienna3);
            vec.push_back(EModel::ToyotaSienna4);
        }
        if (ESerie::ToyotaTacoma == serie)
        {
            vec.push_back(EModel::ToyotaTacoma1);
            vec.push_back(EModel::ToyotaTacoma2);
            vec.push_back(EModel::ToyotaTacoma3);
        }
        if (ESerie::ToyotaTundra == serie)
        {
            vec.push_back(EModel::ToyotaTundra1);
            vec.push_back(EModel::ToyotaTundra2);
        }
        if (ESerie::ToyotaVellfire == serie)
        {
            vec.push_back(EModel::ToyotaVellfire1);
            vec.push_back(EModel::ToyotaVellfire2);
        }
        if (ESerie::ToyotaVitz == serie)
        {
            vec.push_back(EModel::ToyotaVitzP10);
            vec.push_back(EModel::ToyotaVitzXP90);
            vec.push_back(EModel::ToyotaVitzXP130);
        }
        if (ESerie::ToyotaVoxy == serie)
        {
            vec.push_back(EModel::ToyotaVoxyR60);
            vec.push_back(EModel::ToyotaVoxyR70);
            vec.push_back(EModel::ToyotaVoxyR80);
        }
        if (ESerie::ToyotaYaris == serie)
        {
            vec.push_back(EModel::ToyotaYaris);
        }
        if (ESerie::NissanAlmera == serie)
        {
            vec.push_back(EModel::NissanAlmeraN16);
            vec.push_back(EModel::NissanAlmeraG15);
        }
        if (ESerie::NissanAltima == serie)
        {
            vec.push_back(EModel::NissanAltimaL32);
            vec.push_back(EModel::NissanAltimaL33);
            vec.push_back(EModel::NissanAltimaL34);
        }
        if (ESerie::NissanArmada == serie)
        {
            vec.push_back(EModel::NissanArmada1);
            vec.push_back(EModel::NissanArmada2);
        }
        if (ESerie::NissanBluebird == serie)
        {
            vec.push_back(EModel::NissanBluebird);
        }
        if (ESerie::NissanCube == serie)
        {
            vec.push_back(EModel::NissanCubeZ10);
            vec.push_back(EModel::NissanCubeZ11);
            vec.push_back(EModel::NissanCubeZ12);
        }
        if (ESerie::NissanElgrand == serie)
        {
            vec.push_back(EModel::NissanElgrandE50);
            vec.push_back(EModel::NissanElgrandE51);
            vec.push_back(EModel::NissanElgrandE52);
        }
        if (ESerie::NissanFrontier == serie)
        {
            vec.push_back(EModel::NissanFrontier);
        }
        if (ESerie::NissanFuga == serie)
        {
            vec.push_back(EModel::NissanFuga1);
            vec.push_back(EModel::NissanFuga2);
        }
        if (ESerie::NissanJuke == serie)
        {
            vec.push_back(EModel::NissanJuke1);
            vec.push_back(EModel::NissanJuke2);
        }
        if (ESerie::NissanKicks == serie)
        {
            vec.push_back(EModel::NissanKicks1);
        }
        if (ESerie::NissanLatio == serie)
        {
            vec.push_back(EModel::NissanLatioN71);
        }
        if (ESerie::NissanLeaf == serie)
        {
            vec.push_back(EModel::NissanLeafZE0);
            vec.push_back(EModel::NissanLeafZE1);
        }
        if (ESerie::NissanMarch == serie)
        {
            vec.push_back(EModel::NissanMarchK12);
            vec.push_back(EModel::NissanMarchK13);
        }
        if (ESerie::NissanMaxima == serie)
        {
            vec.push_back(EModel::NissanMaximaA34);
            vec.push_back(EModel::NissanMaximaA35);
            vec.push_back(EModel::NissanMaximaA36);
        }
        if (ESerie::NissanMicra == serie)
        {
            vec.push_back(EModel::NissanMicra3);
            vec.push_back(EModel::NissanMicra4);
            vec.push_back(EModel::NissanMicra5);
        }
        if (ESerie::NissanMurano == serie)
        {
            vec.push_back(EModel::NissanMuranoZ50);
            vec.push_back(EModel::NissanMuranoZ51);
            vec.push_back(EModel::NissanMuranoZ52);
        }
        if (ESerie::NissanNote == serie)
        {
            vec.push_back(EModel::NissanNote1);
            vec.push_back(EModel::NissanNote2);
        }
        if (ESerie::NissanPathfinder == serie)
        {
            vec.push_back(EModel::NissanPathfinder3);
            vec.push_back(EModel::NissanPathfinder4);
            vec.push_back(EModel::NissanPathfinder5);
        }
        if (ESerie::NissanPatrol == serie)
        {
            vec.push_back(EModel::NissanPatrolY61);
            vec.push_back(EModel::NissanPatrolY62);
        }
        if (ESerie::NissanPresage == serie)
        {
            vec.push_back(EModel::NissanPresage1);
            vec.push_back(EModel::NissanPresage2);
        }
        if (ESerie::NissanQashqai == serie)
        {
            vec.push_back(EModel::NissanQashqai1);
            vec.push_back(EModel::NissanQashqai2);
            vec.push_back(EModel::NissanQashqai3);
        }
        if (ESerie::NissanQuest == serie)
        {
            vec.push_back(EModel::NissanQuest3);
            vec.push_back(EModel::NissanQuest4);
        }
        if (ESerie::NissanRogue == serie)
        {
            vec.push_back(EModel::NissanRogue1);
            vec.push_back(EModel::NissanRogue2);
            vec.push_back(EModel::NissanRogue3);
        }
        if (ESerie::NissanSentra == serie)
        {
            vec.push_back(EModel::NissanSentraB16);
            vec.push_back(EModel::NissanSentraB17);
            vec.push_back(EModel::NissanSentraB18);
        }
        if (ESerie::NissanSerena == serie)
        {
            vec.push_back(EModel::NissanSerenaC24);
            vec.push_back(EModel::NissanSerenaC25);
            vec.push_back(EModel::NissanSerenaC26);
            vec.push_back(EModel::NissanSerenaC27);
        }
        if (ESerie::NissanSkyline == serie)
        {
            vec.push_back(EModel::NissanSkylineR34);
            vec.push_back(EModel::NissanSkylineV35);
            vec.push_back(EModel::NissanSkylineV36);
            vec.push_back(EModel::NissanSkylineV37);
        }
        if (ESerie::NissanSunny == serie)
        {
            vec.push_back(EModel::NissanSunnyN16);
            vec.push_back(EModel::NissanSunnyB10);
        }
        if (ESerie::NissanTeana == serie)
        {
            vec.push_back(EModel::NissanTeana1);
            vec.push_back(EModel::NissanTeana2);
            vec.push_back(EModel::NissanTeana3);
        }
        if (ESerie::NissanTerrano == serie)
        {
            vec.push_back(EModel::NissanTerrano2);
            vec.push_back(EModel::NissanTerrano3);
        }
        if (ESerie::NissanTiida == serie)
        {
            vec.push_back(EModel::NissanTiida1);
            vec.push_back(EModel::NissanTiida2);
        }
        if (ESerie::NissanVersa == serie)
        {
            vec.push_back(EModel::NissanVersa1);
            vec.push_back(EModel::NissanVersa2);
            vec.push_back(EModel::NissanVersa3);
        }
        if (ESerie::NissanXTrail == serie)
        {
            vec.push_back(EModel::NissanXTrail1);
            vec.push_back(EModel::NissanXTrail2);
            vec.push_back(EModel::NissanXTrail3);
        }
        if (ESerie::NissanXTerra == serie)
        {
            vec.push_back(EModel::NissanXTerra1);
            vec.push_back(EModel::NissanXTerra2);
        }
        if (ESerie::Nissan350Z == serie)
        {
            vec.push_back(EModel::Nissan350Z);
        }
        if (ESerie::FordEdge == serie)
        {
            vec.push_back(EModel::FordEdge1);
            vec.push_back(EModel::FordEdge2);
        }
        if (ESerie::FordEscape == serie)
        {
            vec.push_back(EModel::FordEscape1);
            vec.push_back(EModel::FordEscape2);
            vec.push_back(EModel::FordEscape3);
            vec.push_back(EModel::FordEscape4);
        }
        if (ESerie::FordExplorer == serie)
        {
            vec.push_back(EModel::FordExplorer1);
            vec.push_back(EModel::FordExplorer2);
            vec.push_back(EModel::FordExplorer3);
            vec.push_back(EModel::FordExplorer4);
            vec.push_back(EModel::FordExplorer5);
            vec.push_back(EModel::FordExplorer6);
        }
        if (ESerie::FordF150 == serie)
        {
            vec.push_back(EModel::FordF15010);
            vec.push_back(EModel::FordF15011);
            vec.push_back(EModel::FordF15012);
            vec.push_back(EModel::FordF15013);
        }
        if (ESerie::FordFiesta == serie)
        {
            vec.push_back(EModel::FordFiestaMK6);
            vec.push_back(EModel::FordFiestaMK7);
        }
        if (ESerie::FordFocus == serie)
        {
            vec.push_back(EModel::FordFocus1);
            vec.push_back(EModel::FordFocus2);
            vec.push_back(EModel::FordFocus3);
            vec.push_back(EModel::FordFocus4);
        }
        if (ESerie::FordFusion == serie)
        {
            vec.push_back(EModel::FordFusion2);
        }
        if (ESerie::FordTaurus == serie)
        {
            vec.push_back(EModel::FordTaurus6);
        }
        if (ESerie::FordTransit == serie)
        {
            vec.push_back(EModel::FordTransit);
        }
        if (ESerie::FordMustang == serie)
        {
            vec.push_back(EModel::FordMustang4);
            vec.push_back(EModel::FordMustang5);
            vec.push_back(EModel::FordMustang6);
        }
        if (ESerie::ChevroletAvalanche == serie)
        {
            vec.push_back(EModel::ChevroletAvalanche1);
            vec.push_back(EModel::ChevroletAvalanche2);
        }
        if (ESerie::ChevroletAveo == serie)
        {
            vec.push_back(EModel::ChevroletAveo1);
            vec.push_back(EModel::ChevroletAveo2);
            vec.push_back(EModel::ChevroletAveo3);
        }
        if (ESerie::ChevroletCaptiva == serie)
        {
            vec.push_back(EModel::ChevroletCaptiva1);
        }
        if (ESerie::ChevroletCruze == serie)
        {
            vec.push_back(EModel::ChevroletCruze1);
            vec.push_back(EModel::ChevroletCruze2);
        }
        if (ESerie::ChevroletEpica == serie)
        {
            vec.push_back(EModel::ChevroletEpicaV250);
        }
        if (ESerie::ChevroletEquinox == serie)
        {
            vec.push_back(EModel::ChevroletEquinox1);
            vec.push_back(EModel::ChevroletEquinox2);
            vec.push_back(EModel::ChevroletEquinox3);
        }
        if (ESerie::ChevroletMalibu == serie)
        {
            vec.push_back(EModel::ChevroletMalibu7);
            vec.push_back(EModel::ChevroletMalibu8);
            vec.push_back(EModel::ChevroletMalibu9);
        }
        if (ESerie::ChevroletNiva == serie)
        {
            vec.push_back(EModel::ChevroletNiva);
        }
        if (ESerie::ChevroletTahoe == serie)
        {
            vec.push_back(EModel::ChevroletTahoe2);
            vec.push_back(EModel::ChevroletTahoe3);
            vec.push_back(EModel::ChevroletTahoe4);
            vec.push_back(EModel::ChevroletTahoe5);
        }
        if (ESerie::ChevroletTrailBlazer == serie)
        {
            vec.push_back(EModel::ChevroletTrailBlazer1);
            vec.push_back(EModel::ChevroletTrailBlazer2);
            vec.push_back(EModel::ChevroletTrailBlazer3);
        }
        if (ESerie::ChevroletVolt == serie)
        {
            vec.push_back(EModel::ChevroletVolt1);
            vec.push_back(EModel::ChevroletVolt2);
        }
        if (ESerie::ChevroletCamaro == serie)
        {
            vec.push_back(EModel::ChevroletCamaro5);
            vec.push_back(EModel::ChevroletCamaro6);
        }
        if (ESerie::ChevroletCorvette == serie)
        {
            vec.push_back(EModel::ChevroletCorvette);
        }
        if (ESerie::VolkswagenBeetle == serie)
        {
            vec.push_back(EModel::VolkswagenBeetleA4);
            vec.push_back(EModel::VolkswagenBeetleA5);
        }
        if (ESerie::VolkswagenBora == serie)
        {
            vec.push_back(EModel::VolkswagenBora1998);
            vec.push_back(EModel::VolkswagenBora4);
        }
        if (ESerie::VolkswagenCaddy == serie)
        {
            vec.push_back(EModel::VolkswagenCaddy2);
            vec.push_back(EModel::VolkswagenCaddy3);
            vec.push_back(EModel::VolkswagenCaddy4);
            vec.push_back(EModel::VolkswagenCaddy5);
        }
        if (ESerie::VolkswagenCrafter == serie)
        {
            vec.push_back(EModel::VolkswagenCrafter);
        }
        if (ESerie::VolkswagenGolf == serie)
        {
            vec.push_back(EModel::VolkswagenGolf2);
            vec.push_back(EModel::VolkswagenGolf3);
            vec.push_back(EModel::VolkswagenGolf4);
            vec.push_back(EModel::VolkswagenGolf5);
            vec.push_back(EModel::VolkswagenGolf6);
            vec.push_back(EModel::VolkswagenGolf7);
            vec.push_back(EModel::VolkswagenGolf8);
        }
        if (ESerie::VolkswagenJetta == serie)
        {
            vec.push_back(EModel::VolkswagenJetta3);
            vec.push_back(EModel::VolkswagenJetta4);
            vec.push_back(EModel::VolkswagenJetta5);
            vec.push_back(EModel::VolkswagenJetta6);
            vec.push_back(EModel::VolkswagenJetta7);
        }
        if (ESerie::VolkswagenPassat == serie)
        {
            vec.push_back(EModel::VolkswagenPassatB5);
            vec.push_back(EModel::VolkswagenPassatB6);
            vec.push_back(EModel::VolkswagenPassatB7);
            vec.push_back(EModel::VolkswagenPassatB8);
        }
        if (ESerie::VolkswagenPassatCC == serie)
        {
            vec.push_back(EModel::VolkswagenPassatCC1);
        }
        if (ESerie::VolkswagenPolo == serie)
        {
            vec.push_back(EModel::VolkswagenPolo3);
            vec.push_back(EModel::VolkswagenPolo4);
            vec.push_back(EModel::VolkswagenPolo5);
            vec.push_back(EModel::VolkswagenPolo6);
        }
        if (ESerie::VolkswagenRoutan == serie)
        {
            vec.push_back(EModel::VolkswagenRoutan);
        }
        if (ESerie::VolkswagenTiguan == serie)
        {
            vec.push_back(EModel::VolkswagenTiguan1);
            vec.push_back(EModel::VolkswagenTiguan2);
        }
        if (ESerie::VolkswagenTouareg == serie)
        {
            vec.push_back(EModel::VolkswagenTouareg1);
            vec.push_back(EModel::VolkswagenTouareg2);
            vec.push_back(EModel::VolkswagenTouareg3);
        }
        if (ESerie::VolkswagenTouran == serie)
        {
            vec.push_back(EModel::VolkswagenTouran1);
            vec.push_back(EModel::VolkswagenTouran2);
            vec.push_back(EModel::VolkswagenTouran3);
        }
        if (ESerie::VolkswagenTransporter == serie)
        {
            vec.push_back(EModel::VolkswagenTransporterT4);
            vec.push_back(EModel::VolkswagenTransporterT5);
            vec.push_back(EModel::VolkswagenTransporterT6);
        }
        if (ESerie::VolkswagenVento == serie)
        {
            vec.push_back(EModel::VolkswagenVento);
        }
        if (ESerie::OpelAstra == serie)
        {
            vec.push_back(EModel::OpelAstraF);
            vec.push_back(EModel::OpelAstraG);
            vec.push_back(EModel::OpelAstraH);
            vec.push_back(EModel::OpelAstraJ);
            vec.push_back(EModel::OpelAstraK);
        }
        if (ESerie::OpelCombo == serie)
        {
            vec.push_back(EModel::OpelComboC);
            vec.push_back(EModel::OpelComboD);
            vec.push_back(EModel::OpelComboE);
            vec.push_back(EModel::OpelComboPCMA);
        }
        if (ESerie::OpelCorsa == serie)
        {
            vec.push_back(EModel::OpelCorsaA);
            vec.push_back(EModel::OpelCorsaB);
            vec.push_back(EModel::OpelCorsaC);
            vec.push_back(EModel::OpelCorsaD);
            vec.push_back(EModel::OpelCorsaE);
            vec.push_back(EModel::OpelCorsaF);
        }
        if (ESerie::OpelFrontera == serie)
        {
            vec.push_back(EModel::OpelFronteraA);
            vec.push_back(EModel::OpelFronteraB);
        }
        if (ESerie::OpelInsignia == serie)
        {
            vec.push_back(EModel::OpelInsignia1);
            vec.push_back(EModel::OpelInsignia2);
        }
        if (ESerie::OpelMeriva == serie)
        {
            vec.push_back(EModel::OpelMerivaA);
            vec.push_back(EModel::OpelMerivaB);
        }
        if (ESerie::OpelOmega == serie)
        {
            vec.push_back(EModel::OpelOmegaA);
            vec.push_back(EModel::OpelOmegaB);
        }
        if (ESerie::OpelSignum == serie)
        {
            vec.push_back(EModel::OpelSignum);
        }
        if (ESerie::OpelSintra == serie)
        {
            vec.push_back(EModel::OpelSintra);
        }
        if (ESerie::OpelVectra == serie)
        {
            vec.push_back(EModel::OpelVectraA);
            vec.push_back(EModel::OpelVectraB);
            vec.push_back(EModel::OpelVectraC);
        }
        if (ESerie::OpelZafira == serie)
        {
            vec.push_back(EModel::OpelZafiraA);
            vec.push_back(EModel::OpelZafiraB);
            vec.push_back(EModel::OpelZafiraC);
        }
        if (ESerie::MitsubishiAirtrek == serie)
        {
            vec.push_back(EModel::MitsubishiAirtrek);
        }
        if (ESerie::MitsubishiChariot == serie)
        {
            vec.push_back(EModel::MitsubishiChariot2);
            vec.push_back(EModel::MitsubishiChariot3);
        }
        if (ESerie::MitsubishiColt == serie)
        {
            vec.push_back(EModel::MitsubishiColtZ30);
        }
        if (ESerie::MitsubishiGalant == serie)
        {
            vec.push_back(EModel::MitsubishiGalant8);
            vec.push_back(EModel::MitsubishiGalant9);
        }
        if (ESerie::MitsubishiGrandis == serie)
        {
            vec.push_back(EModel::MitsubishiGrandis);
        }
        if (ESerie::MitsubishiLancer == serie)
        {
            vec.push_back(EModel::MitsubishiLancer9);
            vec.push_back(EModel::MitsubishiLancer10);
        }
        if (ESerie::MitsubishiMontero == serie)
        {
            vec.push_back(EModel::MitsubishiMontero3);
            vec.push_back(EModel::MitsubishiMontero4);
        }
        if (ESerie::MitsubishiOutlander == serie)
        {
            vec.push_back(EModel::MitsubishiOutlander1);
            vec.push_back(EModel::MitsubishiOutlander2);
            vec.push_back(EModel::MitsubishiOutlander3);
        }
        if (ESerie::MitsubishiPajero == serie)
        {
            vec.push_back(EModel::MitsubishiPajero2);
            vec.push_back(EModel::MitsubishiPajero3);
            vec.push_back(EModel::MitsubishiPajero4);
        }
        if (ESerie::KIACadenza == serie)
        {
            vec.push_back(EModel::KIACadenza1);
            vec.push_back(EModel::KIACadenza2);
        }
        if (ESerie::KIACerato == serie)
        {
            vec.push_back(EModel::KIACerato1);
            vec.push_back(EModel::KIACerato2);
            vec.push_back(EModel::KIACerato3);
        }
        if (ESerie::KIAForte == serie)
        {
            vec.push_back(EModel::KIAForte1);
            vec.push_back(EModel::KIAForte2);
            vec.push_back(EModel::KIAForte3);
        }
        if (ESerie::KIAK5 == serie)
        {
            vec.push_back(EModel::KIAK51);
            vec.push_back(EModel::KIAK52);
            vec.push_back(EModel::KIAK53);
        }
        if (ESerie::KIAOptima == serie)
        {
            vec.push_back(EModel::KIAOptima2);
            vec.push_back(EModel::KIAOptima3);
            vec.push_back(EModel::KIAOptima4);
        }
        if (ESerie::KIARio == serie)
        {
            vec.push_back(EModel::KIARio2);
            vec.push_back(EModel::KIARio3);
            vec.push_back(EModel::KIARio4);
        }
        if (ESerie::KIASedona == serie)
        {
            vec.push_back(EModel::KIASedona1);
            vec.push_back(EModel::KIASedona2);
            vec.push_back(EModel::KIASedona3);
        }
        if (ESerie::KIASorento == serie)
        {
            vec.push_back(EModel::KIASorento1);
            vec.push_back(EModel::KIASorento2);
            vec.push_back(EModel::KIASorento3);
            vec.push_back(EModel::KIASorento4);
        }
        if (ESerie::KIASportage == serie)
        {
            vec.push_back(EModel::KIASportage2);
            vec.push_back(EModel::KIASportage3);
            vec.push_back(EModel::KIASportage4);
        }
        if (ESerie::HyundaiAccent == serie)
        {
            vec.push_back(EModel::HyundaiAccent2);
            vec.push_back(EModel::HyundaiAccent3);
            vec.push_back(EModel::HyundaiAccent4);
            vec.push_back(EModel::HyundaiAccent5);
        }
        if (ESerie::HyundaiCreta == serie)
        {
            vec.push_back(EModel::HyundaiCreta1);
            vec.push_back(EModel::HyundaiCreta2);
        }
        if (ESerie::HyundaiElantra == serie)
        {
            vec.push_back(EModel::HyundaiElantra4);
            vec.push_back(EModel::HyundaiElantra5);
            vec.push_back(EModel::HyundaiElantra6);
            vec.push_back(EModel::HyundaiElantra7);
        }
        if (ESerie::HyundaiGenesis == serie)
        {
            vec.push_back(EModel::HyundaiGenesis1);
            vec.push_back(EModel::HyundaiGenesis2);
        }
        if (ESerie::HyundaiGenesisCoupe == serie)
        {
            vec.push_back(EModel::HyundaiGenesisCoupe1);
        }
        if (ESerie::HyundaiIX35 == serie)
        {
            vec.push_back(EModel::HyundaiIX351);
        }
        if (ESerie::HyundaiKona == serie)
        {
            vec.push_back(EModel::HyundaiKona1);
        }
        if (ESerie::HyundaiSantaFe == serie)
        {
            vec.push_back(EModel::HyundaiSantaFe1);
            vec.push_back(EModel::HyundaiSantaFe2);
            vec.push_back(EModel::HyundaiSantaFe3);
            vec.push_back(EModel::HyundaiSantaFe4);
        }
        if (ESerie::HyundaiSolaris == serie)
        {
            vec.push_back(EModel::HyundaiSolaris1);
            vec.push_back(EModel::HyundaiSolaris2);
        }
        if (ESerie::HyundaiSonata == serie)
        {
            vec.push_back(EModel::HyundaiSonata5);
            vec.push_back(EModel::HyundaiSonata6);
            vec.push_back(EModel::HyundaiSonata7);
            vec.push_back(EModel::HyundaiSonata8);
        }
        if (ESerie::HyundaiTucson == serie)
        {
            vec.push_back(EModel::HyundaiTucson1);
            vec.push_back(EModel::HyundaiTucson2);
            vec.push_back(EModel::HyundaiTucson3);
            vec.push_back(EModel::HyundaiTucson4);
        }
        if (ESerie::HyundaiVeloster == serie)
        {
            vec.push_back(EModel::HyundaiVeloster1);
            vec.push_back(EModel::HyundaiVeloster2);
        }
        if (ESerie::HondaAccord == serie)
        {
            vec.push_back(EModel::HondaAccord7);
            vec.push_back(EModel::HondaAccord8);
            vec.push_back(EModel::HondaAccord9);
            vec.push_back(EModel::HondaAccord10);
        }
        if (ESerie::HondaCivic == serie)
        {
            vec.push_back(EModel::HondaCivic7);
            vec.push_back(EModel::HondaCivic8);
            vec.push_back(EModel::HondaCivic9);
            vec.push_back(EModel::HondaCivic10);
            vec.push_back(EModel::HondaCivic11);
        }
        if (ESerie::HondaCRV == serie)
        {
            vec.push_back(EModel::HondaCRV1);
            vec.push_back(EModel::HondaCRV2);
            vec.push_back(EModel::HondaCRV3);
            vec.push_back(EModel::HondaCRV4);
            vec.push_back(EModel::HondaCRV5);
        }
        if (ESerie::HondaCrosstour == serie)
        {
            vec.push_back(EModel::HondaCrosstour1);
        }
        if (ESerie::HondaElyson == serie)
        {
            vec.push_back(EModel::HondaElysion1);
        }
        if (ESerie::HondaFit == serie)
        {
            vec.push_back(EModel::HondaFit1);
            vec.push_back(EModel::HondaFit2);
            vec.push_back(EModel::HondaFit3);
        }
        if (ESerie::HondaHRV == serie)
        {
            vec.push_back(EModel::HondaHRV1);
            vec.push_back(EModel::HondaHRV2);
        }
        if (ESerie::HondaOdyssey == serie)
        {
            vec.push_back(EModel::HondaOdyssey3);
            vec.push_back(EModel::HondaOdyssey4);
            vec.push_back(EModel::HondaOdyssey5);
            vec.push_back(EModel::HondaOdyssey6);
        }
        if (ESerie::HondaPilot == serie)
        {
            vec.push_back(EModel::HondaPilot1);
            vec.push_back(EModel::HondaPilot2);
            vec.push_back(EModel::HondaPilot3);
        }
        if (ESerie::HondaRidgeline == serie)
        {
            vec.push_back(EModel::HondaRidgeline1);
            vec.push_back(EModel::HondaRidgeline2);
        }
        if (ESerie::HondaStepwgn == serie)
        {
            vec.push_back(EModel::HondaStepwgn2);
            vec.push_back(EModel::HondaStepwgn3);
            vec.push_back(EModel::HondaStepwgn4);
            vec.push_back(EModel::HondaStepwgn5);
        }
        if (ESerie::MazdaAtenza == serie)
        {
            vec.push_back(EModel::MazdaAtenza1);
            vec.push_back(EModel::MazdaAtenza2);
            vec.push_back(EModel::MazdaAtenza3);
        }
        if (ESerie::MazdaCX3 == serie)
        {
            vec.push_back(EModel::MazdaCX31);
        }
        if (ESerie::MazdaCX5 == serie)
        {
            vec.push_back(EModel::MazdaCX51);
            vec.push_back(EModel::MazdaCX52);
        }
        if (ESerie::MazdaCX7 == serie)
        {
            vec.push_back(EModel::MazdaCX71);
        }
        if (ESerie::MazdaCX9 == serie)
        {
            vec.push_back(EModel::MazdaCX91);
            vec.push_back(EModel::MazdaCX92);
        }
        if (ESerie::MazdaDemio == serie)
        {
            vec.push_back(EModel::MazdaDemio1);
            vec.push_back(EModel::MazdaDemio2);
            vec.push_back(EModel::MazdaDemio3);
            vec.push_back(EModel::MazdaDemio4);
        }
        if (ESerie::MazdaMPV == serie)
        {
            vec.push_back(EModel::MazdaMPV2);
            vec.push_back(EModel::MazdaMPV3);
        }
        if (ESerie::MazdaRX8 == serie)
        {
            vec.push_back(EModel::MazdaRX8);
        }
        if (ESerie::MazdaTribute == serie)
        {
            vec.push_back(EModel::MazdaTribute1);
            vec.push_back(EModel::MazdaTribute2);
        }
        if (ESerie::Mazda3 == serie)
        {
            vec.push_back(EModel::Mazda32);
            vec.push_back(EModel::Mazda33);
            vec.push_back(EModel::Mazda34);
        }
        if (ESerie::Mazda5 == serie)
        {
            vec.push_back(EModel::Mazda51);
            vec.push_back(EModel::Mazda52);
        }
        if (ESerie::Mazda6 == serie)
        {
            vec.push_back(EModel::Mazda61);
            vec.push_back(EModel::Mazda62);
            vec.push_back(EModel::Mazda63);
        }
        return vec;
    }
};

#endif
