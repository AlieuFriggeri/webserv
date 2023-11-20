/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.class.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:52:34 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/20 19:19:45 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_CLASS_HPP
# define REQUEST_HANDLER_CLASS_HPP

# include "HttpRequest.class.hpp"
# include "HttpRespond.class.hpp"

// # include "Client.hpp"
// # include "CgiExecutor.hpp"
# include "Socket.hpp"

// class Client;
// class CgiExecutor;
// class Socket;


class RequestHandler
{
	private:
		// CONFIG_SERVER	conf;

	public:
		RequestHandler(void);
		// RequestHandler( CONFIG DU SERVER );
		RequestHandler(const RequestHandler& src);
		RequestHandler& operator=(const RequestHandler& src);
		~RequestHandler(void);

		virtual HttpRespond	handleRequest(HttpRequest *req, Client clt, Socket srv) = 0;
};

#endif
