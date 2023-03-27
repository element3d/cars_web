#ifndef __EMAKE__
#define __EMAKE__

#include <string>
#include <vector>
#include "ESerie.h"

class EMake
{
public:
   static std::string None;
   static std::string All;
   static std::string Bmw;
   static std::string MercedesBenz;
   static std::string Audi;
   static std::string Porsche;
   static std::string Lexus;
   static std::string Infiniti;
   static std::string Toyota;
   static std::string Nissan;
   static std::string Ford;
   static std::string Chevrolet;
   static std::string Volkswagen;
   static std::string Opel;
   static std::string Mitsubishi;
   static std::string Kia;
   static std::string Hyundai;
   static std::string Honda;
   static std::string Mazda;

   static std::string toString(const std::string& make)
    {
       if (EMake::Bmw == make) return "BMW";
       if (EMake::MercedesBenz == make) return "MERCEDES BENZ";
       if (EMake::Audi == make) return "AUDI";
       if (EMake::Porsche == make) return "PORSCHE";
       if (EMake::Lexus == make) return "LEXUS";
       if (EMake::Infiniti == make) return "INFINITI";
       if (EMake::Toyota == make) return "TOYOTA";
       if (EMake::Nissan == make) return "NISSAN";
       if (EMake::Ford == make) return "FORD";
       if (EMake::Chevrolet == make) return "CHEVROLET";
       if (EMake::Volkswagen == make) return "VOLKSWAGEN";
       if (EMake::Opel == make) return "OPEL";
       if (EMake::Mitsubishi == make) return "MITSUBISHI";
       if (EMake::Kia == make) return "KIA";
       if (EMake::Hyundai == make) return "HYUNDAI";
       if (EMake::Honda == make) return "HONDA";
       if (EMake::Mazda == make) return "MAZDA";
       return "";
   }

    static std::vector<std::string> ToArray()
    {
        std::vector<std::string> vec;
        vec.push_back(EMake::Bmw);
        vec.push_back(EMake::MercedesBenz);
        vec.push_back(EMake::Audi);
        vec.push_back(EMake::Porsche);
        vec.push_back(EMake::Lexus);
        vec.push_back(EMake::Infiniti);
        vec.push_back(EMake::Toyota);
        vec.push_back(EMake::Nissan);
        vec.push_back(EMake::Ford);
        vec.push_back(EMake::Chevrolet);
        vec.push_back(EMake::Volkswagen);
        vec.push_back(EMake::Opel);
        vec.push_back(EMake::Mitsubishi);
        vec.push_back(EMake::Kia);
        vec.push_back(EMake::Hyundai);
        vec.push_back(EMake::Honda);
        vec.push_back(EMake::Mazda);
        return vec;
    }
    static std::vector<std::string> SeriesToArray(const std::string& make)
    {
        std::vector<std::string> vec;
        if (EMake::Bmw == make)
        {
            vec.push_back(ESerie::Bmw1);
            vec.push_back(ESerie::Bmw2);
            vec.push_back(ESerie::Bmw3);
            vec.push_back(ESerie::Bmw4);
            vec.push_back(ESerie::Bmw5);
            vec.push_back(ESerie::Bmw6);
            vec.push_back(ESerie::Bmw7);
            vec.push_back(ESerie::BmwX1);
            vec.push_back(ESerie::BmwX3);
            vec.push_back(ESerie::BmwX4);
            vec.push_back(ESerie::BmwX5);
            vec.push_back(ESerie::BmwX6);
            vec.push_back(ESerie::BmwX7);
            vec.push_back(ESerie::BmwZ4);
            vec.push_back(ESerie::BmwM);
        }
        if (EMake::MercedesBenz == make)
        {
            vec.push_back(ESerie::MercedesBenz190Class);
            vec.push_back(ESerie::MercedesBenzAClass);
            vec.push_back(ESerie::MercedesBenzBClass);
            vec.push_back(ESerie::MercedesBenzCClass);
            vec.push_back(ESerie::MercedesBenzEClass);
            vec.push_back(ESerie::MercedesBenzSClass);
            vec.push_back(ESerie::MercedesBenzCLClass);
            vec.push_back(ESerie::MercedesBenzCLAClass);
            vec.push_back(ESerie::MercedesBenzCLKClass);
            vec.push_back(ESerie::MercedesBenzCLSClass);
            vec.push_back(ESerie::MercedesBenzMLClass);
            vec.push_back(ESerie::MercedesBenzGClass);
            vec.push_back(ESerie::MercedesBenzGLClass);
            vec.push_back(ESerie::MercedesBenzGLAClass);
            vec.push_back(ESerie::MercedesBenzGLCCLass);
            vec.push_back(ESerie::MercedesBenzGLEClass);
            vec.push_back(ESerie::MercedesBenzGLKClass);
            vec.push_back(ESerie::MercedesBenzGLSClass);
            vec.push_back(ESerie::MercedesBenzRClass);
            vec.push_back(ESerie::MercedesBenzSLClass);
            vec.push_back(ESerie::MercedesBenzSLKClass);
            vec.push_back(ESerie::MercedesBenzVClass);
            vec.push_back(ESerie::MercedesBenzSprinter);
            vec.push_back(ESerie::MercedesBenzVaneo);
            vec.push_back(ESerie::MercedesBenzViano);
            vec.push_back(ESerie::MercedesBenzVito);
            vec.push_back(ESerie::MercedesBenzAMG);
        }
        if (EMake::Audi == make)
        {
            vec.push_back(ESerie::AudiA3);
            vec.push_back(ESerie::AudiA4);
            vec.push_back(ESerie::AudiA5);
            vec.push_back(ESerie::AudiA6);
            vec.push_back(ESerie::AudiA7);
            vec.push_back(ESerie::AudiA8);
            vec.push_back(ESerie::AudiQ5);
            vec.push_back(ESerie::AudiQ7);
            vec.push_back(ESerie::AudiTT);
            vec.push_back(ESerie::AudiRS);
        }
        if (EMake::Porsche == make)
        {
            vec.push_back(ESerie::PorscheCayenne);
            vec.push_back(ESerie::PorscheMacan);
            vec.push_back(ESerie::PorschePanamera);
        }
        if (EMake::Lexus == make)
        {
            vec.push_back(ESerie::LexusCT);
            vec.push_back(ESerie::LexusIS);
            vec.push_back(ESerie::LexusES);
            vec.push_back(ESerie::LexusGS);
            vec.push_back(ESerie::LexusLS);
            vec.push_back(ESerie::LexusNX);
            vec.push_back(ESerie::LexusRX);
            vec.push_back(ESerie::LexusGX);
            vec.push_back(ESerie::LexusLX);
            vec.push_back(ESerie::LexusF);
        }
        if (EMake::Infiniti == make)
        {
            vec.push_back(ESerie::InfinitiEXSeries);
            vec.push_back(ESerie::InfinitiJXSeries);
            vec.push_back(ESerie::InfinitiFXSeries);
            vec.push_back(ESerie::InfinityGSeries);
            vec.push_back(ESerie::InfinityMSeries);
            vec.push_back(ESerie::InfinitiQ3Series);
            vec.push_back(ESerie::InfinitiQX4Series);
            vec.push_back(ESerie::InfinitiQX50Series);
            vec.push_back(ESerie::InfinitiQX56Series);
            vec.push_back(ESerie::InfinitiQX60Series);
            vec.push_back(ESerie::InfinitiQX70Series);
            vec.push_back(ESerie::InfinitiQX80Series);
        }
        if (EMake::Toyota == make)
        {
            vec.push_back(ESerie::ToyotaAlphard);
            vec.push_back(ESerie::ToyotaAltezza);
            vec.push_back(ESerie::ToyotaAvalon);
            vec.push_back(ESerie::ToyotaAvensis);
            vec.push_back(ESerie::ToyotaBelta);
            vec.push_back(ESerie::ToyotaBlade);
            vec.push_back(ESerie::ToyotaCHR);
            vec.push_back(ESerie::ToyotaCamry);
            vec.push_back(ESerie::ToyotaCorolla);
            vec.push_back(ESerie::ToyotaEstima);
            vec.push_back(ESerie::ToyotaFJCruiser);
            vec.push_back(ESerie::ToyotaFortuner);
            vec.push_back(ESerie::ToyotaHarrier);
            vec.push_back(ESerie::ToyotaHighlander);
            vec.push_back(ESerie::ToyotaHilux);
            vec.push_back(ESerie::ToyotaIpsum);
            vec.push_back(ESerie::ToyotaIst);
            vec.push_back(ESerie::ToyotaLandCruiser);
            vec.push_back(ESerie::ToyotaLandCruiserPrado);
            vec.push_back(ESerie::ToyotaNoah);
            vec.push_back(ESerie::ToyotaPasso);
            vec.push_back(ESerie::ToyotaPrius);
            vec.push_back(ESerie::ToyotaRAV4);
            vec.push_back(ESerie::ToyotaSequoia);
            vec.push_back(ESerie::ToyotaSienna);
            vec.push_back(ESerie::ToyotaTacoma);
            vec.push_back(ESerie::ToyotaTundra);
            vec.push_back(ESerie::ToyotaVellfire);
            vec.push_back(ESerie::ToyotaVitz);
            vec.push_back(ESerie::ToyotaVoxy);
            vec.push_back(ESerie::ToyotaYaris);
        }
        if (EMake::Nissan == make)
        {
            vec.push_back(ESerie::NissanAlmera);
            vec.push_back(ESerie::NissanAltima);
            vec.push_back(ESerie::NissanArmada);
            vec.push_back(ESerie::NissanBluebird);
            vec.push_back(ESerie::NissanCube);
            vec.push_back(ESerie::NissanElgrand);
            vec.push_back(ESerie::NissanFrontier);
            vec.push_back(ESerie::NissanFuga);
            vec.push_back(ESerie::NissanJuke);
            vec.push_back(ESerie::NissanKicks);
            vec.push_back(ESerie::NissanLatio);
            vec.push_back(ESerie::NissanLeaf);
            vec.push_back(ESerie::NissanMarch);
            vec.push_back(ESerie::NissanMaxima);
            vec.push_back(ESerie::NissanMicra);
            vec.push_back(ESerie::NissanMurano);
            vec.push_back(ESerie::NissanNote);
            vec.push_back(ESerie::NissanPathfinder);
            vec.push_back(ESerie::NissanPatrol);
            vec.push_back(ESerie::NissanPresage);
            vec.push_back(ESerie::NissanQashqai);
            vec.push_back(ESerie::NissanQuest);
            vec.push_back(ESerie::NissanRogue);
            vec.push_back(ESerie::NissanSentra);
            vec.push_back(ESerie::NissanSerena);
            vec.push_back(ESerie::NissanSkyline);
            vec.push_back(ESerie::NissanSunny);
            vec.push_back(ESerie::NissanTeana);
            vec.push_back(ESerie::NissanTerrano);
            vec.push_back(ESerie::NissanTiida);
            vec.push_back(ESerie::NissanVersa);
            vec.push_back(ESerie::NissanXTrail);
            vec.push_back(ESerie::NissanXTerra);
            vec.push_back(ESerie::Nissan350Z);
        }
        if (EMake::Ford == make)
        {
            vec.push_back(ESerie::FordEdge);
            vec.push_back(ESerie::FordEscape);
            vec.push_back(ESerie::FordExplorer);
            vec.push_back(ESerie::FordF150);
            vec.push_back(ESerie::FordFiesta);
            vec.push_back(ESerie::FordFocus);
            vec.push_back(ESerie::FordFusion);
            vec.push_back(ESerie::FordTaurus);
            vec.push_back(ESerie::FordTransit);
            vec.push_back(ESerie::FordMustang);
        }
        if (EMake::Chevrolet == make)
        {
            vec.push_back(ESerie::ChevroletAvalanche);
            vec.push_back(ESerie::ChevroletAveo);
            vec.push_back(ESerie::ChevroletCaptiva);
            vec.push_back(ESerie::ChevroletCruze);
            vec.push_back(ESerie::ChevroletEpica);
            vec.push_back(ESerie::ChevroletEquinox);
            vec.push_back(ESerie::ChevroletMalibu);
            vec.push_back(ESerie::ChevroletNiva);
            vec.push_back(ESerie::ChevroletTahoe);
            vec.push_back(ESerie::ChevroletTrailBlazer);
            vec.push_back(ESerie::ChevroletVolt);
            vec.push_back(ESerie::ChevroletCamaro);
            vec.push_back(ESerie::ChevroletCorvette);
        }
        if (EMake::Volkswagen == make)
        {
            vec.push_back(ESerie::VolkswagenBeetle);
            vec.push_back(ESerie::VolkswagenBora);
            vec.push_back(ESerie::VolkswagenCaddy);
            vec.push_back(ESerie::VolkswagenCrafter);
            vec.push_back(ESerie::VolkswagenGolf);
            vec.push_back(ESerie::VolkswagenJetta);
            vec.push_back(ESerie::VolkswagenPassat);
            vec.push_back(ESerie::VolkswagenPassatCC);
            vec.push_back(ESerie::VolkswagenPolo);
            vec.push_back(ESerie::VolkswagenRoutan);
            vec.push_back(ESerie::VolkswagenTiguan);
            vec.push_back(ESerie::VolkswagenTouareg);
            vec.push_back(ESerie::VolkswagenTouran);
            vec.push_back(ESerie::VolkswagenTransporter);
            vec.push_back(ESerie::VolkswagenVento);
        }
        if (EMake::Opel == make)
        {
            vec.push_back(ESerie::OpelAstra);
            vec.push_back(ESerie::OpelCombo);
            vec.push_back(ESerie::OpelCorsa);
            vec.push_back(ESerie::OpelFrontera);
            vec.push_back(ESerie::OpelInsignia);
            vec.push_back(ESerie::OpelMeriva);
            vec.push_back(ESerie::OpelOmega);
            vec.push_back(ESerie::OpelSignum);
            vec.push_back(ESerie::OpelSintra);
            vec.push_back(ESerie::OpelVectra);
            vec.push_back(ESerie::OpelZafira);
        }
        if (EMake::Mitsubishi == make)
        {
            vec.push_back(ESerie::MitsubishiAirtrek);
            vec.push_back(ESerie::MitsubishiChariot);
            vec.push_back(ESerie::MitsubishiColt);
            vec.push_back(ESerie::MitsubishiGalant);
            vec.push_back(ESerie::MitsubishiGrandis);
            vec.push_back(ESerie::MitsubishiLancer);
            vec.push_back(ESerie::MitsubishiMontero);
            vec.push_back(ESerie::MitsubishiOutlander);
            vec.push_back(ESerie::MitsubishiPajero);
        }
        if (EMake::Kia == make)
        {
            vec.push_back(ESerie::KIACadenza);
            vec.push_back(ESerie::KIACerato);
            vec.push_back(ESerie::KIAForte);
            vec.push_back(ESerie::KIAK5);
            vec.push_back(ESerie::KIAOptima);
            vec.push_back(ESerie::KIARio);
            vec.push_back(ESerie::KIASedona);
            vec.push_back(ESerie::KIASorento);
            vec.push_back(ESerie::KIASportage);
        }
        if (EMake::Hyundai == make)
        {
            vec.push_back(ESerie::HyundaiAccent);
            vec.push_back(ESerie::HyundaiCreta);
            vec.push_back(ESerie::HyundaiElantra);
            vec.push_back(ESerie::HyundaiGenesis);
            vec.push_back(ESerie::HyundaiGenesisCoupe);
            vec.push_back(ESerie::HyundaiIX35);
            vec.push_back(ESerie::HyundaiKona);
            vec.push_back(ESerie::HyundaiSantaFe);
            vec.push_back(ESerie::HyundaiSolaris);
            vec.push_back(ESerie::HyundaiSonata);
            vec.push_back(ESerie::HyundaiTucson);
            vec.push_back(ESerie::HyundaiVeloster);
        }
        if (EMake::Honda == make)
        {
            vec.push_back(ESerie::HondaAccord);
            vec.push_back(ESerie::HondaCivic);
            vec.push_back(ESerie::HondaCRV);
            vec.push_back(ESerie::HondaCrosstour);
            vec.push_back(ESerie::HondaElyson);
            vec.push_back(ESerie::HondaFit);
            vec.push_back(ESerie::HondaHRV);
            vec.push_back(ESerie::HondaOdyssey);
            vec.push_back(ESerie::HondaPilot);
            vec.push_back(ESerie::HondaRidgeline);
            vec.push_back(ESerie::HondaStepwgn);
        }
        if (EMake::Mazda == make)
        {
            vec.push_back(ESerie::MazdaAtenza);
            vec.push_back(ESerie::MazdaCX3);
            vec.push_back(ESerie::MazdaCX5);
            vec.push_back(ESerie::MazdaCX7);
            vec.push_back(ESerie::MazdaCX9);
            vec.push_back(ESerie::MazdaDemio);
            vec.push_back(ESerie::MazdaMPV);
            vec.push_back(ESerie::MazdaRX8);
            vec.push_back(ESerie::MazdaTribute);
            vec.push_back(ESerie::Mazda3);
            vec.push_back(ESerie::Mazda5);
            vec.push_back(ESerie::Mazda6);
        }
        return vec;
    }
};

#endif
