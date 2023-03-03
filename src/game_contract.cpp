#include <eosio/eosio.hpp>
#include <string>

using namespace eosio;
using namespace std;

class [[eosio::contract("eosiogame")]] store : public contract {
    public:
        using contract::contract;

        struct [[eosio::table("scores")]] scores 
        {
            uint64_t    id;
            string      username;
            uint64_t    score;
            uint64_t primary_key() const { return id; }
        };
        typedef eosio::multi_index<"scores"_n, scores> scores_inst;

        struct [[eosio::table("totalscores")]] totalscores 
        {
            uint64_t    id;
            string      username;
            uint64_t    score;
            uint64_t primary_key() const { return id; }
        };
        typedef eosio::multi_index<"totalscores"_n, totalscores> total_scores_inst;


        //insert scores in table totalscores and scores
        [[eosio::action]] void insertscore(string username, uint64_t score){
            require_auth("eosiogame"_n);

            vector < scores > temp_storage;
            
            
            scores_inst scores_table(get_self(), name("eosiogame").value);
            total_scores_inst total_scores_table(get_self(), name("eosiogame").value);

            //this table contains only 10 off the highest scores
            scores_table.emplace(name("eosiogame"), [&](auto &player){
                player.id       = scores_table.available_primary_key();
                player.username = username;
                player.score    = score;
            });

            //inserting data every time the game is played
            total_scores_table.emplace(name("eosiogame"), [&](auto &player){
                player.id       = total_scores_table.available_primary_key();
                player.username = username;
                player.score    = score;
            });

            uint64_t dist = distance(scores_table.begin(), scores_table.end());
            temp_storage.reserve(dist);

            for(auto scores_itr = scores_table.begin(); scores_itr != scores_table.end() && temp_storage.size() < dist; scores_itr++){
                temp_storage.emplace_back(scores{
                    .id = scores_itr->id,
                    .username = scores_itr->username,
                    .score = scores_itr->score
                });
            }

            sort(temp_storage.begin(), temp_storage.end(), [](const scores& left, const scores& right){
                return left.score > right.score;
            });

            for(auto scores_itr = temp_storage.begin(); scores_itr != temp_storage.end(); scores_itr++){
                auto rmv_it = scores_table.find(scores_itr->id);
                scores_table.erase(rmv_it);
            }

            uint16_t num = 0;

            for(auto tmp_itr = temp_storage.begin(); tmp_itr != temp_storage.end() && num <= 10; tmp_itr++){
                scores_table.emplace(name("eosiogame"), [&](auto &player){
                    player.id       = scores_table.available_primary_key();
                    player.username = tmp_itr->username;
                    player.score    = tmp_itr->score;
                });
                num++;
            }
        }

        //NOTE clearscores depends on how much EOSIO resurces does ones account have
        //if cpu resources overloads or trxs time the action will fail
        [[eosio::action]] void clearscores(){
            require_auth("eosiogame"_n);

            scores_inst scores_table(get_self(), name("eosiogame").value);
            vector< scores > rmv_scores;
            uint64_t dist = distance(scores_table.begin(), scores_table.end());
            rmv_scores.reserve(dist);
            
            for(auto it = scores_table.begin(); it != scores_table.end(); it++)
                rmv_scores.emplace_back(scores{
                    .id = it->id,
                    .username = it->username,
                    .score = it->score
                });

            for(auto it = rmv_scores.begin(); it != rmv_scores.end(); it++){
                auto rmv_it = scores_table.find(it->id);
                if(rmv_it != scores_table.end())
                    scores_table.erase(rmv_it);
            }

            total_scores_inst total_scores_table(get_self(), name("eosiogame").value);
            vector< scores > rmv_total;
            dist = distance(total_scores_table.begin(), total_scores_table.end());
            rmv_total.reserve(dist);
            
            for(auto it = total_scores_table.begin(); it != total_scores_table.end(); it++)
                rmv_total.emplace_back(scores{
                    .id = it->id,
                    .username = it->username,
                    .score = it->score
                });

            for(auto it = rmv_total.begin(); it != rmv_total.end(); it++){
                auto rmv_it = total_scores_table.find(it->id);
                if(rmv_it != total_scores_table.end())
                    total_scores_table.erase(rmv_it);
            }
                
        }   
};
